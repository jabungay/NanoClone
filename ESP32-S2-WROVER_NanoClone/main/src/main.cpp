
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "driver/gpio.h"

#include "pwm.h"

#include "wifi.h"
#include "http.h"
#include "panel.h"

PWM tPWM(GPIO_NUM_6, LEDC_CHANNEL_0, LEDC_TIMER_0);

Nanoclone n;
void xTaskNanoclone(void * pvParams)
{
    for(;;)
    {
        static uint8_t b = 255;
        static bool up = false;
        n.vSetBrightness(b);

        if (up) { b++; }
        else    { b--; }

        if (b == 255 || b == 0) { up = !up; }

        n.vTasks(10); // Pass the time delta into the Nanoclone task so that it can update at a consistent speed
        vTaskDelay(pdMS_TO_TICKS(10)); // Don't let this task run too often (only need to update LEDs every ~10ms) 
    }
}

void xTaskTest(void * pvParams)
{
    for (;;)
    {
        tPWM.vUpdate();
        vTaskDelay(pdMS_TO_TICKS(D_TIME_PER_STEP_PWM));
    }
}

extern "C"
{
    void app_main(void)
    {
        vStripInit();
        // vWifiInit();
        // vHttpInit();

        printf("hi\n");

        // n = Nanoclone(NUM_PANELS, LEDS_PER_PANEL, xTaskNanoclone);
        // n.vSetColour(0, {0,0,255});

        gpio_config_t gpio = {
            GPIO_SEL_8,
            GPIO_MODE_OUTPUT,
            GPIO_PULLUP_DISABLE,
            GPIO_PULLDOWN_DISABLE,
            GPIO_INTR_DISABLE
        };

        gpio_config(&gpio);

        // Task for LED handling
        static uint8_t param;
        TaskHandle_t h = NULL;
        xTaskCreate(xTaskTest, "TaskTest", 2048, &param, tskIDLE_PRIORITY, &h);

        tPWM.vStartPWM();

        tPWM.vQueueDuty(255);
        vTaskDelay(pdMS_TO_TICKS(2000));
        tPWM.vQueueDuty(0);




    }
}

