//////////////////////////////////////////////////////////////
// pwm.cpp - Class to interface with ESP32 ledc PWM controller
// (c) Jonathan Bungay - 20210103
//////////////////////////////////////////////////////////////

#include "pwm.h"

ledc_isr_handle_t handle;

PWM::PWM(uint32_t gpio, ledc_channel_t channel, ledc_timer_t tmr)
{
    // Set up PWM channel
    this->_sChannelConfig.gpio_num   = gpio;
    this->_sChannelConfig.speed_mode = LEDC_LOW_SPEED_MODE;
    this->_sChannelConfig.channel    = channel;
    this->_sChannelConfig.intr_type  = LEDC_INTR_FADE_END;
    this->_sChannelConfig.timer_sel  = tmr;
    this->_sChannelConfig.duty       = 0;                       // Start at 0 duty cycle (off)
    this->_sChannelConfig.hpoint     = 0;

    // Set up PWM timer
    this->_sTimerConfig.speed_mode      = LEDC_LOW_SPEED_MODE;
    this->_sTimerConfig.duty_resolution = LEDC_TIMER_8_BIT;
    this->_sTimerConfig.timer_num       = tmr;
    this->_sTimerConfig.freq_hz         = 5000;                 // Use 5kHz PWM signal
    this->_sTimerConfig.clk_cfg         = LEDC_AUTO_CLK;

    this->_bActive = false;
}

PWM::~PWM() { }

// Start the PWM controller
void PWM::vStartPWM(void)
{
    esp_err_t errTmr = ledc_timer_config(&(this->_sTimerConfig));
    esp_err_t errPwm = ledc_channel_config(&(this->_sChannelConfig));

    ledc_fade_func_install(0);  // Start the interrupt-based fade function

    if (errTmr == ESP_OK && errPwm == ESP_OK) { this->_bActive = false; printf("PWM Started Successfully\n"); }
    else { printf("Failed to start PWM\n"); }
}

// Set a duty cycle right now
void PWM::vSetDuty(uint8_t uDuty)
{
    ledc_set_duty_and_update(this->_sChannelConfig.speed_mode, this->_sChannelConfig.channel, uDuty, this->_sChannelConfig.hpoint );
}

// Get the current duty cycle directly from the ledc peripheral
uint8_t PWM::uGetDuty(void)
{
    uint8_t uDuty = (uint8_t) ledc_get_duty(this->_sChannelConfig.speed_mode, this->_sChannelConfig.channel);
    return(uDuty);
}

// Queue a target duty cycle
// NOTE: Not using the built-in fade due to problems
// with fade and inability to interrupt a current fade
void PWM::vQueueDuty(uint8_t uDuty)
{
    this->_uTargetDuty  = uDuty;
    this->_dCurrentDuty = (double) this->uGetDuty();
    this->_dDeltaDuty   = ((double)_uTargetDuty - _dCurrentDuty) / (double)D_FADE_STEPS_PWM;

    // Reset step count and setup new task
    this->_uStep    = 0;
    this->_bNewTask = true;
}

// Update the PWM controller
// should be called repeatedly (but with correct timing)
void PWM::vUpdate(void)
{
    if (this->_bNewTask)
    {
        vSetDuty((uint8_t)_dCurrentDuty);   // Set the duty cycle immediately
        _dCurrentDuty += _dDeltaDuty;       // Increment the duty cycle

        _uStep++;
        if (_uStep >= D_FADE_STEPS_PWM) { vSetDuty(_uTargetDuty); _bNewTask = false; printf("Fade Done [0x%02X]\n", _uTargetDuty); }    // Stop the task when we're done
    }

}