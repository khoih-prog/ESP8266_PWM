/****************************************************************************************************************************
  multiFileProject.ino
  
  For ESP8266 boards
  Written by Khoi Hoang

  Built by Khoi Hoang https://github.com/khoih-prog/ESP8266_PWM
  Licensed under MIT license
*****************************************************************************************************************************/

// To demo how to include files in multi-file Projects

#if !defined(ESP8266)
  #error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

#define ESP8266_PWM_VERSION_MIN_TARGET      "ESP8266_PWM v1.2.4"
#define ESP8266_PWM_VERSION_MIN             1002004

#include "multiFileProject.h"

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ESP8266_PWM.h"

void setup() 
{
  Serial.begin(115200);
  while (!Serial);
  
  Serial.println("\nStart multiFileProject");
  Serial.println(ESP8266_PWM_VERSION);

#if defined(ESP8266_PWM_VERSION_MIN)
  if (ESP8266_PWM_VERSION_INT < ESP8266_PWM_VERSION_MIN)
  {
    Serial.print("Warning. Must use this example on Version equal or later than : ");
    Serial.println(ESP8266_PWM_VERSION_MIN_TARGET);
  }
#endif
}

void loop() 
{
  // put your main code here, to run repeatedly:
}
