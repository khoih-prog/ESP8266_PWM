/****************************************************************************************************************************
  ESP8266_PWM_ISR.hpp
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

  Version: 1.2.4

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      21/09/2021 Initial coding for ESP8266 boards with ESP8266 core v3.0.2+
  1.1.0   K Hoang      06/11/2021 Add functions to modify PWM settings on-the-fly
  1.2.0   K Hoang      29/01/2022 Fix multiple-definitions linker error. Improve accuracy
  1.2.1   K Hoang      30/01/2022 Fix bug. Optimize code
  1.2.2   K Hoang      30/01/2022 DutyCycle to be updated at the end current PWM period
  1.2.3   K Hoang      01/02/2022 Use float for DutyCycle and Freq, uint32_t for period
  1.2.4   K Hoang      04/03/2022 Fix `DutyCycle` and `New Period` display bugs. Display warning only when debug level > 3
*****************************************************************************************************************************/

#pragma once

#ifndef PWM_ISR_GENERIC_HPP
#define PWM_ISR_GENERIC_HPP

#if !defined(ESP8266)
  #error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

#ifndef ESP8266_PWM_VERSION
  #define ESP8266_PWM_VERSION           "ESP8266_PWM v1.2.4"
  
  #define ESP8266_PWM_VERSION_MAJOR     1
  #define ESP8266_PWM_VERSION_MINOR     2
  #define ESP8266_PWM_VERSION_PATCH     4

  #define ESP8266_PWM_VERSION_INT       1002004
#endif

#ifndef TIMER_INTERRUPT_DEBUG
  #define TIMER_INTERRUPT_DEBUG      0
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

  #if (_PWM_LOGLEVEL_ > 3)
    #warning Not USING_MICROS_RESOLUTION, using millis resolution
  #endif
    
  #define USING_MICROS_RESOLUTION       false
#endif

#if !defined(CHANGING_PWM_END_OF_CYCLE)
  #if (_PWM_LOGLEVEL_ > 3)
    #warning Using default CHANGING_PWM_END_OF_CYCLE == true
  #endif
  
  #define CHANGING_PWM_END_OF_CYCLE     true
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
    // Return the channelNum if OK, -1 if error
    int setPWM(const uint32_t& pin, const float& frequency, const float& dutycycle, esp8266_timer_callback StartCallback = nullptr, 
                esp8266_timer_callback StopCallback = nullptr)
    {
      uint32_t period = 0;
      
      if ( ( frequency > 0.0 ) && ( frequency <= 500.0 ) )
      {
#if USING_MICROS_RESOLUTION
      // period in us
      period = (uint32_t) (1000000.0f / frequency);
#else    
      // period in ms
      period = (uint32_t) (1000.0f / frequency);
#endif
      }
      else
      {       
        PWM_LOGERROR("Error: Invalid frequency, max is 500Hz");
        
        return -1;
      }
      
      return setupPWMChannel(pin, period, dutycycle, (void *) StartCallback, (void *) StopCallback);      
    }

    // period in us
    // Return the channelNum if OK, -1 if error
    int setPWM_Period(const uint32_t& pin, const uint32_t& period, const float& dutycycle, 
                      esp8266_timer_callback StartCallback = nullptr, esp8266_timer_callback StopCallback = nullptr)  
    {     
      return setupPWMChannel(pin, period, dutycycle, (void *) StartCallback, (void *) StopCallback);      
    }    
    
    //////////////////////////////////////////////////////////////////
    
    // low level function to modify a PWM channel
    // returns the true on success or false on failure
    bool modifyPWMChannel(const uint8_t& channelNum, const uint32_t& pin, const float& frequency, const float& dutycycle)
    {
      uint32_t period = 0;
      
      if ( ( frequency > 0.0 ) && ( frequency <= 500.0 ) )
      {
#if USING_MICROS_RESOLUTION
      // period in us
      period = (uint32_t) (1000000.0f / frequency);
#else    
      // period in ms
      period = (uint32_t) (1000.0f / frequency);
#endif
      }
      else
      {       
        PWM_LOGERROR("Error: Invalid frequency, max is 500Hz");
        return false;
      }
      
      return modifyPWMChannel_Period(channelNum, pin, period, dutycycle);
    }
    
    //period in us
    bool modifyPWMChannel_Period(const uint8_t& channelNum, const uint32_t& pin, const uint32_t& period, const float& dutycycle);

    // destroy the specified PWM channel
    void deleteChannel(const uint8_t& channelNum);

    // restart the specified PWM channel
    void restartChannel(const uint8_t& channelNum);

    // returns true if the specified PWM channel is enabled
    bool isEnabled(const uint8_t& channelNum);

    // enables the specified PWM channel
    void enable(const uint8_t& channelNum);

    // disables the specified PWM channel
    void disable(const uint8_t& channelNum);

    // enables all PWM channels
    void enableAll();

    // disables all PWM channels
    void disableAll();

    // enables the specified PWM channel if it's currently disabled, and vice-versa
    void toggle(const uint8_t& channelNum);

    // returns the number of used PWM channels
    int8_t getnumChannels();

    // returns the number of available PWM channels
    uint8_t getNumAvailablePWMChannels() 
    {
      if (numChannels <= 0)
        return MAX_NUMBER_CHANNELS;
      else 
        return MAX_NUMBER_CHANNELS - numChannels;
    };

  private:

    // low level function to initialize and enable a new PWM channel
    // returns the PWM channel number (channelNum) on success or
    // -1 on failure (f == NULL) or no free PWM channels 
    int setupPWMChannel(const uint32_t& pin, const uint32_t& period, const float& dutycycle, void* cbStartFunc = nullptr, void* cbStopFunc = nullptr);
    

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
      
      // New from v1.3.0     
      uint32_t      newPeriod;          // period value, in us / ms
      uint32_t      newOnTime;          // onTime value, ( period * dutyCycle / 100 ) us  / ms
      float         newDutyCycle;       // from 0.00 to 100.00, float precision
      //////
    } PWM_t;

    volatile PWM_t PWM[MAX_NUMBER_CHANNELS];

    // actual number of PWM channels in use (-1 means uninitialized)
    volatile int8_t numChannels;
};


//#include "ESP8266_PWM_ISR.hpp"

#endif    // PWM_ISR_GENERIC_H


