#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_tls.h"

#include "i2c.h"
#include "src/ccs811.h"

void app_main(void)
{	printf("\n	-- CCS811 lib test --\n");	

	init_i2c();

    //Read ID del sensor
    printf("\n	CCS811 ID Number: 0x%x\n", ccs811_read_byte(0x20));

    if (init_ccs811(true)){
        printf("\nInitialized!!!\n");
    }
    else{
        printf("\nNO Initialized\n");
    }
}
