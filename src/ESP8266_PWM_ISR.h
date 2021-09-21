/****************************************************************************************************************************
  ESP8266_PWM_ISR.h
  For ESP8266 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_PWM
  Licensed under MIT license

  The ESP8266 timers are badly designed, using only 23-bit counter along with maximum 256 prescaler. They're only better than UNO / Mega.
  The ESP8266 has two hardware timers, but timer0 has been used for WiFi and it's not advisable to use. Only timer1 is available.
  The timer1's 23-bit counter terribly can count only up to 8,388,607. So the timer1 maximum interval is very short.
  Using 256 prescaler, maximum timer1 interval is only 26.843542 seconds !!!

  Now with these new 16 ISR-based timers, the maximum interval is practically unlimited (limited only by unsigned long miliseconds)
  The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers
  Therefore, their executions are not blocked by bad-behaving functions / tasks.
  This important feature is absolutely necessary for mission-critical tasks.

  Version: 1.0.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      21/09/2021 Initial coding for ESP8266 boards with ESP8266 core v3.0.2+
*****************************************************************************************************************************/

#pragma once

#ifndef PWM_ISR_GENERIC_H
#define PWM_ISR_GENERIC_H

#if !defined(ESP8266)
  #error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

#ifndef ESP8266_PWM_VERSION
  #define ESP8266_PWM_VERSION       "ESP8266_PWM v1.0.0"
#endif

#ifndef _PWM_LOGLEVEL_
  #define _PWM_LOGLEVEL_       1
#endif

#include "PWM_Generic_Debug.h"

#define CONFIG_ESP32_APPTRACE_ENABLE

#include <stddef.h>

#ifdef ESP8266
  extern "C"
  {
    #include "ets_sys.h"
    #include "os_type.h"
    #include "mem.h"
  }
#else
  #include <inttypes.h>
#endif

#if defined(ARDUINO)
  #if ARDUINO >= 100
    #include <Arduino.h>
  #else
    #include <WProgram.h>
  #endif
#endif

#define ESP8266_PWM_ISR ESP8266_PWM

typedef void (*esp8266_timer_callback)();
typedef void (*esp8266_timer_callback_p)(void *);

#if !defined(USING_MICROS_RESOLUTION)
  #warning Not USING_MICROS_RESOLUTION, using millis resolution
  #define USING_MICROS_RESOLUTION       false
#endif

class ESP8266_PWM_ISR 
{

  public:
    // maximum number of PWM channels
#define MAX_NUMBER_CHANNELS        16

    // constructor
    ESP8266_PWM_ISR();

    void init();

    // this function must be called inside loop()
    void IRAM_ATTR run();
    
    //////////////////////////////////////////////////////////////////
    // PWM
    void setPWM(uint32_t pin, uint32_t frequency, uint32_t dutycycle, esp8266_timer_callback StartCallback = nullptr, 
                esp8266_timer_callback StopCallback = nullptr)
    {
      uint32_t period = 0;
      
      if ( ( frequency != 0 ) && ( frequency <= 500 ) )
      {
#if USING_MICROS_RESOLUTION
      // period in us
      period = 1000000 / frequency;
#else    
      // period in ms
      period = 1000 / frequency;
#endif
      }
      else
      {       
        PWM_LOGERROR("Error: Invalid frequency, max is 500Hz");
      }
      
      setupPWMChannel(pin, period, dutycycle, (void *) StartCallback, (void *) StopCallback);      
    }

#if USING_MICROS_RESOLUTION
    //period in us
    void setPWM_Period(uint32_t pin, uint32_t period, uint32_t dutycycle, esp8266_timer_callback StartCallback = nullptr,
                       esp8266_timer_callback StopCallback = nullptr)
#else    
    // PWM
    //period in ms
    void setPWM_Period(uint32_t pin, uint32_t period, uint32_t dutycycle, esp8266_timer_callback StartCallback = nullptr,
                       esp8266_timer_callback StopCallback = nullptr)
#endif    
    {     
      setupPWMChannel(pin, period, dutycycle, (void *) StartCallback, (void *) StopCallback);      
    }    
    
    //////////////////////////////////////////////////////////////////

    // destroy the specified PWM channel
    void deleteChannel(unsigned channelNum);

    // restart the specified PWM channel
    void restartChannel(unsigned channelNum);

    // returns true if the specified PWM channel is enabled
    bool isEnabled(unsigned channelNum);

    // enables the specified PWM channel
    void enable(unsigned channelNum);

    // disables the specified PWM channel
    void disable(unsigned channelNum);

    // enables all PWM channels
    void enableAll();

    // disables all PWM channels
    void disableAll();

    // enables the specified PWM channel if it's currently disabled, and vice-versa
    void toggle(unsigned channelNum);

    // returns the number of used PWM channels
    unsigned getnumChannels();

    // returns the number of available PWM channels
    unsigned getNumAvailablePWMChannels() 
    {
      return MAX_NUMBER_CHANNELS - numChannels;
    };

  private:

    // low level function to initialize and enable a new PWM channel
    // returns the PWM channel number (channelNum) on success or
    // -1 on failure (f == NULL) or no free PWM channels 
    int setupPWMChannel(uint32_t pin, uint32_t period, uint32_t dutycycle, void* cbStartFunc = nullptr, void* cbStopFunc = nullptr);

    // find the first available slot
    int findFirstFreeSlot();

    typedef struct 
    {
      ///////////////////////////////////
      
      
      ///////////////////////////////////
      
      uint64_t      prevTime;           // value returned by the micros() or millis() function in the previous run() call
      uint32_t      period;             // period value, in us / ms
      uint32_t      onTime;             // onTime value, ( period * dutyCycle / 100 ) us  / ms
      
      void*         callbackStart;      // pointer to the callback function when PWM pulse starts (HIGH)
      void*         callbackStop;       // pointer to the callback function when PWM pulse stops (LOW)
      
      ////////////////////////////////////////////////////////////
      
      uint32_t      pin;                // PWM pin
      bool          pinHigh;            // true if PWM pin is HIGH
      ////////////////////////////////////////////////////////////
      
      bool          enabled;            // true if enabled
    } PWM_t;

    volatile PWM_t PWM[MAX_NUMBER_CHANNELS];

    // actual number of PWM channels in use (-1 means uninitialized)
    volatile int numChannels;
};


#include "ESP8266_PWM_ISR.hpp"

#endif    // PWM_ISR_GENERIC_H


