#include "NanoClone.h"

typedef enum
{
    EStateOff,
    EStateColour,
    EStateWhite,
    EStateInvalid
} ENanocloneState;

PWM tLedCW(D_CW_PIN, D_CW_CHANNEL, D_CW_TIMER);   // Cold White LED controller
PWM tLedWW(D_WW_PIN, D_WW_CHANNEL, D_WW_TIMER);   // Warm White LED controller

Nanoclone tNanoClone(D_NUM_TILES, D_LEDS_PER_TILE); // Colour LED controller

// Task for White LEDs
void xTaskLedWhite(void * pvParams)
{
    for (;;)
    {
        // Update the PWM controllers
        tLedCW.vUpdate();
        tLedWW.vUpdate();

        vTaskDelay(pdMS_TO_TICKS(D_TIME_PER_STEP_PWM)); // Suspend the task for a time equal to the time per PWM step
    }
}

// Task for colour LEDs
void xTaskLedColour(void * pvParams)
{
    for (;;)
    {
        static uint8_t b = 255;
        static bool up = false;
        tNanoClone.vSetBrightness(b);

        if (up) { b++; }
        else    { b--; }

        if (b == 255 || b == 0) { up = !up; }

        tNanoClone.vTasks(10); // Pass the time delta into the Nanoclone task so that it can update at a consistent speed
        vTaskDelay(pdMS_TO_TICKS(10)); // Don't let this task run too often (only need to update LEDs every ~10ms) 
    }
}

void vInit_Nanoclone(void)
{
    tNanoClone.vInit();
        // // Initialize White LED task
        // static uint8_t paramLedWhite;
        // TaskHandle_t hLedWhite = NULL;
        // xTaskCreate(xTaskLedWhite, "White LED", 2048, &paramLedWhite, tskIDLE_PRIORITY, &hLedWhite);

    // Initialize Colour LED Task
    static uint8_t paramLedColour;
    static TaskHandle_t hLedColour = NULL;
    xTaskCreate(xTaskLedColour, "Colour LED", 2048, &paramLedColour, tskIDLE_PRIORITY, &hLedColour);

    tNanoClone.vSetColour(0, {0,255,0});
}
