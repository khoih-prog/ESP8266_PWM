/****************************************************************************************************************************
  PWM_GENERIC_Debug.h
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

#ifndef PWM_GENERIC_DEBUG_H
#define PWM_GENERIC_DEBUG_H

#ifdef PWM_GENERIC_DEBUG_PORT
  #define PWM_DBG_PORT      PWM_GENERIC_DEBUG_PORT
#else
  #define PWM_DBG_PORT      Serial
#endif

// Change _PWM_LOGLEVEL_ to set tracing and logging verbosity
// 0: DISABLED: no logging
// 1: ERROR: errors
// 2: WARN: errors and warnings
// 3: INFO: errors, warnings and informational (default)
// 4: DEBUG: errors, warnings, informational and debug

#ifndef _PWM_LOGLEVEL_
  #define _PWM_LOGLEVEL_       1
#endif

#define PWM_LOGERROR(x)         if(_PWM_LOGLEVEL_>0) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.println(x); }
#define PWM_LOGERROR0(x)        if(_PWM_LOGLEVEL_>0) { PWM_DBG_PORT.print(x); }
#define PWM_LOGERRORLN0(x)      if(_PWM_LOGLEVEL_>0) { PWM_DBG_PORT.println(x); }
#define PWM_LOGERROR1(x,y)      if(_PWM_LOGLEVEL_>0) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(y); }
#define PWM_LOGERROR2(x,y,z)    if(_PWM_LOGLEVEL_>0) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(z); }
#define PWM_LOGERROR3(x,y,z,w)  if(_PWM_LOGLEVEL_>0) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(z); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(w); }

#define PWM_LOGWARN(x)          if(_PWM_LOGLEVEL_>1) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.println(x); }
#define PWM_LOGWARN0(x)         if(_PWM_LOGLEVEL_>1) { PWM_DBG_PORT.print(x); }
#define PWM_LOGWARNLN0(x)       if(_PWM_LOGLEVEL_>1) { PWM_DBG_PORT.println(x); }
#define PWM_LOGWARN1(x,y)       if(_PWM_LOGLEVEL_>1) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(y); }
#define PWM_LOGWARN2(x,y,z)     if(_PWM_LOGLEVEL_>1) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(z); }
#define PWM_LOGWARN3(x,y,z,w)   if(_PWM_LOGLEVEL_>1) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(z); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(w); }

#define PWM_LOGINFO(x)          if(_PWM_LOGLEVEL_>2) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.println(x); }
#define PWM_LOGINFO0(x)         if(_PWM_LOGLEVEL_>2) { PWM_DBG_PORT.print(x); }
#define PWM_LOGINFOLN0(x)       if(_PWM_LOGLEVEL_>2) { PWM_DBG_PORT.println(x); }
#define PWM_LOGINFO1(x,y)       if(_PWM_LOGLEVEL_>2) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(y); }
#define PWM_LOGINFO2(x,y,z)     if(_PWM_LOGLEVEL_>2) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(z); }
#define PWM_LOGINFO3(x,y,z,w)   if(_PWM_LOGLEVEL_>2) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(z); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(w); }

#define PWM_LOGDEBUG(x)         if(_PWM_LOGLEVEL_>3) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.println(x); }
#define PWM_LOGDEBUG0(x)        if(_PWM_LOGLEVEL_>3) { PWM_DBG_PORT.print(x); }
#define PWM_LOGDEBUGLN0(x)      if(_PWM_LOGLEVEL_>3) { PWM_DBG_PORT.println(x); }
#define PWM_LOGDEBUG1(x,y)      if(_PWM_LOGLEVEL_>3) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(y); }
#define PWM_LOGDEBUG2(x,y,z)    if(_PWM_LOGLEVEL_>3) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(z); }
#define PWM_LOGDEBUG3(x,y,z,w)  if(_PWM_LOGLEVEL_>3) { PWM_DBG_PORT.print("[PWM] "); PWM_DBG_PORT.print(x); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(y); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.print(z); PWM_DBG_PORT.print(" "); PWM_DBG_PORT.println(w); }

#endif    //PWM_GENERIC_DEBUG_H
