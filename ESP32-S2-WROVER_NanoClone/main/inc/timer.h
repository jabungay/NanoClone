// timer.h : Functions specific to timing
// Delays, etc
// (c) Jonathan Bungay, 20210213

#ifndef _TIMER_H
#define _TIMER_H

#include "esp_system.h"

void vTimerInit(void);

void vWait_us(uint64_t timeUs);
void vWait_ms(uint64_t timeMs);

#endif /* _TIMER_H */