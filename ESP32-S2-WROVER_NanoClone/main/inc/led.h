// led.h
// (c) Jonathan Bungay, 20210213

#include "globals.h"

#ifndef _LED_H
#define _LED_H

// Configuration options for LEDs
#define LEDS_PER_PANEL 24
#define NUM_PANELS 2
#define NUM_LEDS (LEDS_PER_PANEL * NUM_PANELS)

#define LED_PIN GPIO_NUM_10
#define LED_RMT_CH RMT_CHANNEL_0

// Timing requirements for RGB LED
#define WS2812_T0H_NS (350)
#define WS2812_T0L_NS (1000)
#define WS2812_T1H_NS (1000)
#define WS2812_T1L_NS (350)
#define WS2812_RESET_US (280)

// Structure that contains colour data for single RGB LED
typedef struct 
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} SRGB;

#endif /* _LED_H */