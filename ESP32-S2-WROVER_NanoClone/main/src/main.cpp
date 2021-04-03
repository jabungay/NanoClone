
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "driver/gpio.h"

#include "wifi.h"
#include "http.h"

#include "NanoClone.h"

// Nanoclone n;
// void xTaskNanoclone(void * pvParams)
// {
//     for(;;)
//     {
//         static uint8_t b = 255;
//         static bool up = false;
//         n.vSetBrightness(b);

//         if (up) { b++; }
//         else    { b--; }

//         if (b == 255 || b == 0) { up = !up; }

//         n.vTasks(10); // Pass the time delta into the Nanoclone task so that it can update at a consistent speed
//         vTaskDelay(pdMS_TO_TICKS(10)); // Don't let this task run too often (only need to update LEDs every ~10ms) 
//     }
// }

extern "C"
{
    void app_main(void)
    {
        vInit_Nanoclone();
        // void xTaskLedColour(void * pvParams);
        // static uint8_t paramLedColour;
        // TaskHandle_t hLedColour = NULL;
        // xTaskCreate(xTaskLedColour, "Colour LED", 2048, &paramLedColour, tskIDLE_PRIORITY, &hLedColour);
        // vStripInit();
        // vWifiInit();
        // vHttpInit();
    }
}

