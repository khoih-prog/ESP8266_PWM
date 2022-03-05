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

#ifndef PWM_ISR_GENERIC_H
#define PWM_ISR_GENERIC_H

#include "ESP8266_PWM_ISR.hpp"
#include "ESP8266_PWM_ISR_Impl.h"

#endif    // PWM_ISR_GENERIC_H


