//////////////////////////////////////////////////////////////
// pwm.h - Class to interface with ESP32 ledc PWM controller
// (c) Jonathan Bungay - 20210103
//////////////////////////////////////////////////////////////
#include "driver/ledc.h"
#include "globals.h"

#define D_FADE_STEPS_PWM 100 // Default PWM fade steps
#define D_FADE_TIME_PWM  500 // How long to fade between steps
#define D_TIME_PER_STEP_PWM (D_FADE_TIME_PWM / D_FADE_STEPS_PWM) // How long to dwell on each step

class PWM
{
public:
    PWM(uint32_t gpio, ledc_channel_t channel, ledc_timer_t tmr);
    ~PWM();

    void vUpdate(void);

    // Start the PWM controller
    void vStartPWM(void);
    void vStopPWM(void);

    void    vSetDuty(uint8_t uDuty);
    void    vQueueDuty(uint8_t uDuty);
    uint8_t uGetDuty(void);
private:
    ledc_channel_config_t _sChannelConfig;
    ledc_timer_config_t   _sTimerConfig;

    bool _bActive; // Is the PWM controller active?

    // Variables for duty fade
    bool     _bNewTask;
    uint8_t  _uTargetDuty;
    double   _dDeltaDuty, _dCurrentDuty;
    uint16_t _uStep;
};