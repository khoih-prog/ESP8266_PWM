
/****************************************************************************************************************************
  ESP8266_PWM.h
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

  Version: 1.1.0

  Version Modified By   Date      Comments
  ------- -----------  ---------- -----------
  1.0.0   K Hoang      21/09/2021 Initial coding for ESP8266 boards with ESP8266 core v3.0.2+
  1.1.0   K Hoang      06/11/2021 Add functions to modify PWM settings on-the-fly
*****************************************************************************************************************************/

#pragma once

#ifndef ESP8266_PWM_H
#define ESP8266_PWM_H

#include "ESP8266_PWM.hpp"
#include "ESP8266_PWM_ISR.h"

#endif    // ESP8266_PWM_H

