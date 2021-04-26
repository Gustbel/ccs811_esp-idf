#include <stdlib.h>
#include <stdio.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "sdkconfig.h"
#include "ccs811.h"

char buffer[100];
char* s;

short value;
i2c_cmd_handle_t cmd;

bool check_ccs811()
{
//	int ret;
//	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
//    i2c_master_start(cmd);
//    i2c_master_write_byte(cmd, MPU6050_SENSOR_ADDR << 1 | WRITE_BIT, ACK_CHECK_EN);
//    i2c_master_write_byte(cmd, MPU6050_SENSOR_ADDR, ACK_CHECK_EN);
//    i2c_master_stop(cmd);
//    ret = i2c_master_cmd_begin(I2C_PORT_NUMBER, cmd, TICK_DELAY);
//    i2c_cmd_link_delete(cmd);
//    if (ret == ESP_OK)
//    	return true;
//    else
    	return true;
}

bool init_ccs811(bool _ft)
{
    bool e;
    e = ccs811_write_byte(CCS811_REG_APP_START, 0); // Put 'No data' (0) in APP_START register for init sensor

    vTaskDelay(100/portTICK_PERIOD_MS);

    return e;
} 

void print_ccs811()
{
    if (check_ccs811())
	printf("ccs811 : OK");
    else
	printf("ccs811 : Not detected (Accelerometer, Gyroscope sensor)");
}

char* get_ccs811(int a)
{
//  	s = ""; 
//    switch (a) 
//    {	
//    	case 0:	/* Aceler X */
//    	sprintf(buffer, "%.2f",( ((slave_read_byte(ACCEL_XOUT_H) << 8) | slave_read_byte(ACCEL_XOUT_H + 1) ) / 1638.4 ) );
//    	s=buffer;
//        return s ; 
//        case 1:	/* Aceler Y */
//    	sprintf(buffer, "%.2f",( ((slave_read_byte(ACCEL_YOUT_H) << 8) | slave_read_byte(ACCEL_YOUT_H + 1) ) / 1638.4 ) );
//    	s=buffer;
//        return s ; 
//        case 2:
//		sprintf(buffer, "%.2f",( ((slave_read_byte(ACCEL_ZOUT_H) << 8) | slave_read_byte(ACCEL_ZOUT_H + 1) ) / 1638.4 ) );
//    	s=buffer;
//        return s ; /* Aceler Z */
//        case 3:
//    	sprintf(buffer, "%.2f",( ((slave_read_byte(GYRO_XOUT_H) << 8) | slave_read_byte(GYRO_XOUT_H + 1) ) / 131.0 ) );
//    	s=buffer;
//        return s ; /* Gyrosc X */
//        case 4:
//    	sprintf(buffer, "%.2f",( ((slave_read_byte(GYRO_YOUT_H) << 8) | slave_read_byte(GYRO_YOUT_H + 1) ) / 131.0 ) );
//    	s=buffer;
//        return s ; /* Gyrosc Y */
//        case 5:
//    	sprintf(buffer, "%.2f",( ((slave_read_byte(GYRO_ZOUT_H) << 8) | slave_read_byte(GYRO_ZOUT_H + 1) ) / 131.0 ) );
//    	s=buffer;
//        return s ; /* Gyrosc Z */
//    }
    return "0";
}

bool ccs811_write_byte(uint8_t reg_addr, uint8_t data)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, CCS811_I2C_ADDRESS_1 << 1 | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, reg_addr, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    int ret = i2c_master_cmd_begin(I2C_PORT_NUMBER, cmd, TICK_DELAY);
    i2c_cmd_link_delete(cmd);
    if (ret == ESP_FAIL) {
        return false;
    }
    return true;
}

// bool slave_write_byte(uint8_t reg_addr, uint8_t data) {
// //    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
// //    i2c_master_start(cmd);
// //    i2c_master_write_byte(cmd, MPU6050_SENSOR_ADDR << 1, ACK_CHECK_EN);
// //    i2c_master_write_byte(cmd, reg_addr, ACK_CHECK_EN);
// //    i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
// //    i2c_master_stop(cmd);
// //    int ret = i2c_master_cmd_begin(I2C_PORT_NUMBER, cmd, TICK_DELAY);
// //    i2c_cmd_link_delete(cmd);
// //    if (ret == ESP_FAIL) {
// //        return false;
// //    }
//     return true;
// }

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


