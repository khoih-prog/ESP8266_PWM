/****************************************************************************************************************************
  multiFileProject.h
  
  For ESP8266 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_PWM
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#pragma once

#define USING_MICROS_RESOLUTION       true    //false

// Default is true, uncomment to false
//#define CHANGING_PWM_END_OF_CYCLE     false

// Select a Timer Clock
#define USING_TIM_DIV1                true              // for shortest and most accurate timer
#define USING_TIM_DIV16               false             // for medium time and medium accurate timer
#define USING_TIM_DIV256              false             // for longest timer but least accurate. Default

// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "ESP8266_PWM.hpp"
