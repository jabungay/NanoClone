/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

#include "driver/gpio.h"
#include "driver/timer.h"

void vWaitUS(uint64_t timeUs)
{
    // What is the first value of the timer?
    uint64_t uTimerCurrentVal = 0;
    timer_get_counter_value(0, 0, &uTimerCurrentVal);

    // What is the target time?
    uint64_t uTimerTargetVal = uTimerCurrentVal + timeUs; 
    
    // Update current val until it is greater than or equal to the target val
    while (uTimerCurrentVal < uTimerTargetVal) 
    {
        timer_get_counter_value(0, 0, &uTimerCurrentVal);
    }

}

void vWaitMS(uint64_t timeMs)
{
    vWaitUS(timeMs * 1000);
}

void app_main(void)
{
    gpio_config_t gTest;
    gTest.intr_type = GPIO_INTR_DISABLE;
    gTest.mode = GPIO_MODE_OUTPUT;
    gTest.pin_bit_mask = (1 << 19); // Configure GPIO 18
    gTest.pull_down_en = 0;
    gTest.pull_up_en = 0;
    gpio_config(&gTest);


    timer_config_t tTest;
    tTest.alarm_en    = TIMER_ALARM_DIS;
    tTest.counter_en  = TIMER_PAUSE;
    tTest.intr_type   = TIMER_INTR_NONE;
    tTest.counter_dir = TIMER_COUNT_UP;
    tTest.auto_reload  = TIMER_AUTORELOAD_EN;
    tTest.divider     = 40;  // Clk=40MHz -> divider 40000 = 1kHz timer
    timer_init(0, 0, &tTest);
    timer_start(0,0);

    while (1)
    {
        // timer_get_counter_value(0, 0, &timerVal);
        // printf("%llu\n", timerVal);

        gpio_set_level(19, 1);
        vWaitMS(500);
        gpio_set_level(19, 0);
        vWaitMS(500);
    }
}
