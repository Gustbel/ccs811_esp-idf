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

void app_main(void) {

    printf("\n	-- CCS811 lib test --\n");	

    // Init I2C
    init_i2c();

    // Init Sensor and set mode
    if (init_ccs811()){
        printf("\nInitialized!!!\n");
    }
    else{
        printf("\nNO Initialized\n");
    }

    // Read data
    while(1){
        printf("CO2: %f", get_ccs811(0));
        vTaskDelay(15/portTICK_PERIOD_MS);
        printf(" - VOC: %f\n", get_ccs811(1));
        vTaskDelay(1500/portTICK_PERIOD_MS);
    }
}
