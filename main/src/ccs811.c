#include <stdlib.h>
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "ccs811.h"

#define CCS811_MODE 1  // CCS811 DRIVE_MODE (See Datasheet)

short value;
i2c_cmd_handle_t cmd;

bool init_ccs811()
{
    const uint8_t app_start = 0;

    if (ccs811_read_byte(CCS811_REG_HW_ID) != 0x81)
        return false;
//    else
//        printf("\n	CCS811 ID Number: 0x%x\n", ccs811_read_byte(CCS811_REG_HW_ID));

    ccs811_write_byte(CCS811_REG_APP_START, app_start, 0); // Put 'No data' (0) in APP_START register for init sensor

    vTaskDelay(100/portTICK_PERIOD_MS);

    // Read Status
//    printf("-   Status: %d\n", ccs811_read_byte(CCS811_REG_STATUS) >> 7);

    vTaskDelay(100/portTICK_PERIOD_MS);

    // Define Mode
    const uint8_t mode = CCS811_MODE << 4;
    ccs811_write_byte(CCS811_REG_MEAS_MODE, mode, 8); // Put 'No data' (0) in APP_START register for init sensor
    vTaskDelay(100/portTICK_PERIOD_MS);

    // Read Mode
//    printf("-   Mode: %d\n", ccs811_read_byte(CCS811_REG_MEAS_MODE) >> 4);

    vTaskDelay(1000/portTICK_PERIOD_MS);

    return true;
}

float get_ccs811(int a)
 {
    ccs811_data d = ccs811_sensor_data(CCS811_REG_ALG_RESULT_DATA);
    switch (a) 
    {	
        case 0:	/* CO2 */
            return d.dataCO2; 
        case 1:	/* VOC */
            return d.dataVOC;
    }
    return 0.0;
}

ccs811_data ccs811_sensor_data(uint8_t addr)
{
    ccs811_data data;
    uint8_t dataCO2_HB, dataCO2_LB, dataVOC_HB, dataVOC_LB;
    
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();	// Command link Create
    i2c_master_start(cmd);						// Start bit 
    i2c_master_write_byte(cmd, CCS811_I2C_ADDRESS_1 << 1 | WRITE_BIT, ACK_CHECK_EN);		// Write an single byte address
    i2c_master_write_byte(cmd, addr, ACK_CHECK_EN);
    i2c_master_stop(cmd);	// Stop bit
    i2c_master_cmd_begin(I2C_PORT_NUMBER, cmd, TICK_DELAY);
    i2c_cmd_link_delete(cmd);
    vTaskDelay(30 / portTICK_RATE_MS);
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, CCS811_I2C_ADDRESS_1 << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, &dataCO2_HB, ACK_VAL);
    i2c_master_read_byte(cmd, &dataCO2_LB, ACK_VAL);
    i2c_master_read_byte(cmd, &dataVOC_HB, ACK_VAL);
    i2c_master_read_byte(cmd, &dataVOC_LB, ACK_VAL);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_PORT_NUMBER, cmd, TICK_DELAY);
    i2c_cmd_link_delete(cmd);

    data.dataCO2 = (dataCO2_HB << 8) | dataCO2_LB;
    data.dataVOC = (dataVOC_HB << 8) | dataVOC_LB;

    return data;
}

bool ccs811_write_byte(uint8_t reg_addr, const uint8_t data, const uint8_t len)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, CCS811_I2C_ADDRESS_1 << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, reg_addr, ACK_CHECK_EN);
    i2c_master_write(cmd, &data, len, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    int ret = i2c_master_cmd_begin(I2C_PORT_NUMBER, cmd, TICK_DELAY);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return false;
    }
    return true;
}

uint8_t ccs811_read_byte(uint8_t addr)
{
    uint8_t data;
    
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();	// Command link Create
    i2c_master_start(cmd);						// Start bit 
    i2c_master_write_byte(cmd, CCS811_I2C_ADDRESS_1 << 1 | WRITE_BIT, ACK_CHECK_EN);		// Write an single byte address
    i2c_master_write_byte(cmd, addr, ACK_CHECK_EN);
    i2c_master_stop(cmd);	// Stop bit
    i2c_master_cmd_begin(I2C_PORT_NUMBER, cmd, TICK_DELAY);
    i2c_cmd_link_delete(cmd);
    vTaskDelay(30 / portTICK_RATE_MS);
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, CCS811_I2C_ADDRESS_1 << 1 | READ_BIT, ACK_CHECK_EN);
    i2c_master_read_byte(cmd, &data, ACK_VAL);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(I2C_PORT_NUMBER, cmd, TICK_DELAY);
    i2c_cmd_link_delete(cmd);

    return data;
}