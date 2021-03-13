
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "driver/gpio.h"

#include "led.h"
#include "wifi.h"
#include "http.h"

void vTaskLED(void * pvParams);
void vStripInit(void);

extern "C"
{
    
    void app_main(void)
    {
        vStripInit();
        // vWifiInit();
        // vHttpInit();

        // SRGB c = {0,0,255};

        // Nanoleaf n = Nanoleaf(1, 1, GPIO_NUM_18);
        // n.setColour(0, c);
        // n.show();


        // Task for LED handling
        static uint8_t ucParameterToPassLED;
        TaskHandle_t xHandleLED = NULL;
        xTaskCreate(vTaskLED, "TaskLED", 2048, &ucParameterToPassLED, tskIDLE_PRIORITY, &xHandleLED);
    }
}

