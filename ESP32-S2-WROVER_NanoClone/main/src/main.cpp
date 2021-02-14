/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "timer.h"
#include "led.h"

#include "wifi.h"

extern "C"
{
    
    void app_main(void)
    {
        vStripInit();
        vWifiInit();
    
        // Task for LED handling
        static uint8_t ucParameterToPassLED;
        TaskHandle_t xHandleLED = NULL;
        xTaskCreate(vTaskLED, "TaskLED", 2048, &ucParameterToPassLED, tskIDLE_PRIORITY, &xHandleLED);
    }
}

