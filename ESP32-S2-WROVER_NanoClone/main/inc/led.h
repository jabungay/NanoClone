// led.h
// (c) Jonathan Bungay, 20210213

#include "globals.h"

#ifndef _LED_H
#define _LED_H

// Configuration options for LEDs
#define LEDS_PER_PANEL 35
#define NUM_PANELS 1
#define NUM_LEDS (LEDS_PER_PANEL * NUM_PANELS)

#define LED_PIN GPIO_NUM_11
#define LED_RMT_CH RMT_CHANNEL_0

// Timing requirements for RGB LED
#define WS2812_T0H_NS   (350) // 0 High Time
#define WS2812_T0L_NS  (1000) // 0  Low Time
#define WS2812_T1H_NS  (1000) // 1 High Time
#define WS2812_T1L_NS   (350) // 1  Low Time
#define WS2812_RESET_US (280) //    Set Time

// Structure that contains colour data for single RGB LED
typedef struct 
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} SRGB;

void vSetPixel(uint8_t index, SRGB *psRGB, SRGB rgb);
void vUpdateLEDs(SRGB psRGB[NUM_LEDS]);

void vStripInit(void);
void xTaskLED(void * pvParams);

#endif /* _LED_H */