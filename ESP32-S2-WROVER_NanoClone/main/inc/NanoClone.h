//////////////////////////////////////////////////////////////
// NanoClone.h - Control NanoClone's LEDs 
//     Both non-adressable white and addressable colour          
// (c) Jonathan Bungay - 20210103
//////////////////////////////////////////////////////////////

#include "pwm.h"        // White LEDs
#include "panel.h"      // Colour LEDs

// DEFINES for addressable LEDs
#define D_NUM_TILES     1
#define D_LEDS_PER_TILE 24

// DEFINES for PWM Controllers
// Cold White
#define D_CW_PIN       (GPIO_NUM_6)
#define D_CW_TIMER     (LEDC_TIMER_0)
#define D_CW_CHANNEL   (LEDC_CHANNEL_0)
// Warm White
#define D_WW_PIN       (GPIO_NUM_7)
#define D_WW_TIMER     (LEDC_TIMER_0) 
#define D_WW_CHANNEL   (LEDC_CHANNEL_1)

void vInit_Nanoclone   (void);
void vUpdate_Nanoclone (void);

