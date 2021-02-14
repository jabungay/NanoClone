// led.h
// (c) Jonathan Bungay, 20210213

#ifndef _LED_H
#define _LED_H

#include "freertos/task.h"

void vStripInit(void);
void vTaskLED(void * pvParams);

#endif /* _LED_H */