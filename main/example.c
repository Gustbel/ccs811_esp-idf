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
    if (init_ccs811(true)){
        printf("\nInitialized!!!\n");
    }
    else{
        printf("\nNO Initialized\n");
    }

    while(1){
        printf("CO2: %s", get_ccs811(0));
        vTaskDelay(15/portTICK_PERIOD_MS);
        printf(" - VOS: %s\n", get_ccs811(1));
        vTaskDelay(1500/portTICK_PERIOD_MS);
    }
}
