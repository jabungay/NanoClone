#include "timer.h"
#include "driver/timer.h"   // ESP32 Timer Libraries

void vTimerInit(void)
{
    timer_config_t tTest;
    tTest.alarm_en    = TIMER_ALARM_DIS;
    tTest.counter_en  = TIMER_PAUSE;
    tTest.intr_type   = TIMER_INTR_LEVEL;
    tTest.counter_dir = TIMER_COUNT_UP;
    tTest.auto_reload = TIMER_AUTORELOAD_EN;
    tTest.divider     = 40;  // Clk=40MHz -> divider 40 = 1MHz timer (microseconds)
    timer_init(TIMER_GROUP_0, TIMER_0, &tTest);
    timer_start(TIMER_GROUP_0,TIMER_0);
}

// Wait some number of microseconds
// NOTE: don't have to take into account potential integer overflow,
// because 2^64 us > 500 000 years
void vWait_us(uint64_t timeUs)
{
    // What is the first value of the timer?
    uint64_t uTimerCurrentVal = 0;
    timer_get_counter_value(TIMER_GROUP_0, TIMER_0, &uTimerCurrentVal);

    // What is the target time?
    uint64_t uTimerTargetVal = uTimerCurrentVal + timeUs; 
    
    // Update current val until it is greater than or equal to the target val
    while (uTimerCurrentVal < uTimerTargetVal) 
    {
        timer_get_counter_value(TIMER_GROUP_0, TIMER_0, &uTimerCurrentVal);
    }

}

void vWait_ms(uint64_t timeMs)
{
    vWait_us(timeMs * 1000);
}