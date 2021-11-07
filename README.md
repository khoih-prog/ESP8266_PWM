# ESP8266_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP8266_PWM.svg?)](https://www.ardu-badge.com/ESP8266_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP8266_PWM.svg)](https://github.com/khoih-prog/ESP8266_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP8266_PWM/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP8266_PWM.svg)](http://github.com/khoih-prog/ESP8266_PWM/issues)

---
---

## Table of Contents

* [Why do we need this ESP8266_PWM library](#why-do-we-need-this-ESP8266_PWM-library)
  * [Features](#features)
  * [Why using ISR-based PWM-channels is better](#Why-using-ISR-based-PWM-channels-is-better)
  * [Currently supported Boards](#currently-supported-boards)
  * [Important Notes about ISR](#important-notes-about-isr)
* [Changelog](changelog.md) 
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)
* [More useful Information](#more-useful-information)
* [Examples](#examples)
  * [ 1. ISR_16_PWMs_Array](examples/ISR_16_PWMs_Array)
  * [ 2. ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex)
  * [ 3. ISR_16_PWMs_Array_Simple](examples/ISR_16_PWMs_Array_Simple)
  * [ 4. ISR_Changing_PWM](examples/ISR_Changing_PWM)
  * [ 5. ISR_Modify_PWM](examples/ISR_Modify_PWM)
* [Example ISR_16_PWMs_Array_Complex](#Example-ISR_16_PWMs_Array_Complex)
* [Debug Terminal Output Samples](#debug-terminal-output-samples)
  * [1. ISR_16_PWMs_Array_Complex on ESP8266_NODEMCU_ESP12E](#1-ISR_16_PWMs_Array_Complex-on-ESP8266_NODEMCU_ESP12E)
  * [2. ISR_16_PWMs_Array on ESP8266_NODEMCU_ESP12E](#2-ISR_16_PWMs_Array-on-ESP8266_NODEMCU_ESP12E)
  * [3. ISR_16_PWMs_Array_Simple on ESP8266_NODEMCU_ESP12E](#3-ISR_16_PWMs_Array_Simple-on-ESP8266_NODEMCU_ESP12E)
  * [4. ISR_Modify_PWM on ESP8266_NODEMCU_ESP12E](#4-ISR_Modify_PWM-on-ESP8266_NODEMCU_ESP12E)
  * [5. ISR_Changing_PWM on ESP8266_NODEMCU_ESP12E](#5-ISR_Changing_PWM-on-ESP8266_NODEMCU_ESP12E)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Why do we need this [ESP8266_PWM library](https://github.com/khoih-prog/ESP8266_PWM)

## Features

This library enables you to use Interrupt from Hardware Timers on an ESP8266-based board to create and output PWM to pins. The maximum PWM frequency is currently limited at **500Hz**. Now you can also modify PWM settings on-the-fly.

---

This library enables you to use Interrupt from Hardware Timers on an ESP8266-based board to create and output PWM to pins. It now supports 16 ISR-based **synchronized** PWM channels, while consuming only 1 Hardware Timer. PWM interval can be very long (uint32_t millisecs). The most important feature is they're ISR-based PWM channels. Therefore, their executions are not blocked by bad-behaving functions or tasks. This important feature is absolutely necessary for mission-critical tasks. These hardware PWM channels, using interrupt, still work even if other functions are blocking. Moreover, they are much more precise (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

As **Hardware Timers are rare, and very precious assets** of any board, this library now enables you to use up to **16 ISR-based synchronized PWM channels, while consuming only 1 Hardware Timer**. Timers' interval is very long (**ulong millisecs**).

Now with these new **16 ISR-based timers**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds) while **the accuracy is nearly perfect** compared to software timers. 

The most important feature is they're ISR-based PWM channels. Therefore, their executions are **not blocked by bad-behaving functions / tasks**. This important feature is absolutely necessary for mission-critical tasks. 

The [**ISR_16_PWMs_Array_Complex**](examples/ISR_16_PWMs_Array_Complex) example will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of PWM channels.

Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use.

This non-being-blocked important feature is absolutely necessary for mission-critical tasks.


### Why using ISR-based PWM-channels is better

Imagine you have a system with a **mission-critical** function, measuring water level and control the sump pump or doing something much more important. You normally use a software timer to poll, or even place the function in loop(). But what if another function is **blocking** the loop() or setup().

So your function **might not be executed, and the result would be disastrous.**

You'd prefer to have your function called, no matter what happening with other functions (busy loop, bug, etc.).

The correct choice is to use a Hardware PWM-channels with **Interrupt** to call your function.

These hardware timers, using interrupt, still work even if other functions are blocking. Moreover, they are much more **precise** (certainly depending on clock frequency accuracy) than other software timers using millis() or micros(). That's necessary if you need to measure some data requiring better accuracy.

Functions using normal software timers, relying on loop() and calling millis(), won't work if the loop() or setup() is blocked by certain operation. For example, certain function is blocking while it's connecting to WiFi or some services.

The catch is **your function is now part of an ISR (Interrupt Service Routine), and must be lean / mean, and follow certain rules.** More to read on:

[**HOWTO Attach Interrupt**](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/)

---

### Currently supported Boards

1. ESP8266 boards, such as **ESP8266_NODEMCU_ESP12E**, etc.


---

### Important Notes about ISR

1. Inside the attached function, **delay() won’t work and the value returned by millis() will not increment.** Serial data received while in the function may be lost. You should declare as **volatile any variables that you modify within the attached function.**

2. Typically global variables are used to pass data between an ISR and the main program. To make sure variables shared between an ISR and the main program are updated correctly, declare them as volatile.


---
---

## Prerequisites

1. [`Arduino IDE 1.8.16+` for Arduino](https://www.arduino.cc/en/Main/Software)
2. [`ESP8266 Core 3.0.2+`](https://github.com/esp8266/Arduino) for ESP8266-based boards. [![Latest release](https://img.shields.io/github/release/esp8266/Arduino.svg)](https://github.com/esp8266/Arduino/releases/latest/)
3. [`SimpleTimer library`](https://github.com/jfturcot/SimpleTimer) to use with some examples.


---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**ESP8266_PWM**](https://github.com/khoih-prog/ESP8266_PWM), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/ESP8266_PWM.svg?)](https://www.ardu-badge.com/ESP8266_PWM) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**ESP8266_PWM**](https://github.com/khoih-prog/ESP8266_PWM) page.
2. Download the latest release `ESP8266_PWM-master.zip`.
3. Extract the zip file to `ESP8266_PWM-master` directory 
4. Copy whole `ESP8266_PWM-master` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**ESP8266_PWM** library](https://platformio.org/lib/show/12847/ESP8266_PWM) by using [Library Manager](https://platformio.org/lib/show/12847/ESP8266_PWM/installation). Search for **ESP8266_PWM** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)


---
---


### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using **xyz-Impl.h instead of standard xyz.cpp**, possibly creates certain `Multiple Definitions` Linker error in certain use cases. Although it's simple to just modify several lines of code, either in the library or in the application, the library is adding 2 more source directories

1. **scr_h** for new h-only files
2. **src_cpp** for standard h/cpp files

besides the standard **src** directory.

To use the **old standard cpp** way, locate this library' directory, then just 

1. **Delete the all the files in src directory.**
2. **Copy all the files in src_cpp directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.

To re-use the **new h-only** way, just 

1. **Delete the all the files in src directory.**
2. **Copy the files in src_h directory into src.**
3. Close then reopen the application code in Arduino IDE, etc. to recompile from scratch.


---
---

## More useful Information

The ESP8266 timers are **badly designed**, using only 23-bit counter along with maximum 256 prescaler. They're only better than UNO / Mega.
The ESP8266 has two hardware timers, but timer0 has been used for WiFi and it's not advisable to use. Only timer1 is available.
The timer1's 23-bit counter terribly can count only up to 8,388,607. So the timer1 maximum interval is very short.
Using 256 prescaler, maximum timer1 interval is only **26.843542 seconds !!!**

The timer1 counters can be configured to support automatic reload.

---

Now with these new **`16 ISR-based timers`**, the maximum interval is **practically unlimited** (limited only by unsigned long miliseconds).

The accuracy is nearly perfect compared to software timers. The most important feature is they're ISR-based timers. Therefore, their executions are not blocked by bad-behaving functions / tasks.

This important feature is absolutely necessary for mission-critical tasks. 

The [ISR_Timer_Complex example](examples/ISR_Timer_Complex) will demonstrate the nearly perfect accuracy compared to software timers by printing the actual elapsed millisecs of each type of timers.

Being ISR-based timers, their executions are not blocked by bad-behaving functions / tasks, such as connecting to WiFi, Internet and Blynk services. You can also have many `(up to 16)` timers to use. This non-being-blocked important feature is absolutely necessary for mission-critical tasks. 

You'll see blynkTimer Software is blocked while system is connecting to WiFi / Internet / Blynk, as well as by blocking task in loop(), using delay() function as an example. The elapsed time then is very unaccurate


---
---

### Examples: 

 1. [ISR_16_PWMs_Array](examples/ISR_16_PWMs_Array)
 2. [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex)
 3. [ISR_16_PWMs_Array_Simple](examples/ISR_16_PWMs_Array_Simple)
 4. [ISR_Changing_PWM](examples/ISR_Changing_PWM)
 5. [ISR_Modify_PWM](examples/ISR_Modify_PWM) 

---
---

### Example [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex)

```
#if !defined(ESP8266)
  #error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP8266_PWM.h"
// _PWM_LOGLEVEL_ from 0 to 4
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_      4

#define USING_MICROS_RESOLUTION       true    //false 

#include "ESP8266_PWM.h"

#include <SimpleTimer.h>              // https://github.com/jfturcot/SimpleTimer

#ifndef LED_BUILTIN
  #define LED_BUILTIN       2
#endif

#define HW_TIMER_INTERVAL_US      20L

volatile uint32_t startMicros = 0;

// Init ESP8266Timer
ESP8266Timer ITimer;

// Init ESP8266_ISR_PWM
ESP8266_PWM ISR_PWM;

void IRAM_ATTR TimerHandler()
{
  ISR_PWM.run();
}

/////////////////////////////////////////////////

#define NUMBER_ISR_PWMS         8

//PIN_D0 can't be used for PWM/I2C
#define PIN_D0            16        // Pin D0 mapped to pin GPIO16/USER/WAKE of ESP8266. This pin is also used for Onboard-Blue LED. PIN_D0 = 0 => LED ON
#define PIN_D1            5         // Pin D1 mapped to pin GPIO5 of ESP8266
#define PIN_D2            4         // Pin D2 mapped to pin GPIO4 of ESP8266
#define PIN_D3            0         // Pin D3 mapped to pin GPIO0/FLASH of ESP8266
#define PIN_D4            2         // Pin D4 mapped to pin GPIO2/TXD1 of ESP8266
//#define PIN_LED           2         // Pin D4 mapped to pin GPIO2/TXD1 of ESP8266, NodeMCU and WeMoS, control on-board LED
#define PIN_D5            14        // Pin D5 mapped to pin GPIO14/HSCLK of ESP8266
#define PIN_D6            12        // Pin D6 mapped to pin GPIO12/HMISO of ESP8266
#define PIN_D7            13        // Pin D7 mapped to pin GPIO13/RXD2/HMOSI of ESP8266
#define PIN_D8            15        // Pin D8 mapped to pin GPIO15/TXD2/HCS of ESP8266
#define PIN_D9            3         // Pin D9 /RX mapped to pin GPIO3/RXD0 of ESP8266
#define PIN_D10           1         // Pin D10/TX mapped to pin GPIO1/TXD0 of ESP8266

//Don't use pins GPIO6 to GPIO11 as already connected to flash, etc. Use them can crash the program
//GPIO9(D11/SD2) and GPIO11 can be used only if flash in DIO mode ( not the default QIO mode)
#define PIN_D11           9         // Pin D11/SD2 mapped to pin GPIO9/SDD2 of ESP8266
#define PIN_D12           10        // Pin D12/SD3 mapped to pin GPIO10/SDD3 of ESP8266

typedef void (*irqCallback)  ();

//////////////////////////////////////////////////////

#define USE_COMPLEX_STRUCT      true

#define USING_PWM_FREQUENCY     true

//////////////////////////////////////////////////////

#if USE_COMPLEX_STRUCT

typedef struct
{
  uint32_t      PWM_Pin;
  irqCallback   irqCallbackStartFunc;
  irqCallback   irqCallbackStopFunc;

#if USING_PWM_FREQUENCY  
  uint32_t      PWM_Freq;
#else  
  uint32_t      PWM_Period;
#endif
  
  uint32_t      PWM_DutyCycle;
  
  uint64_t      deltaMicrosStart;
  uint64_t      previousMicrosStart;

  uint64_t      deltaMicrosStop;
  uint64_t      previousMicrosStop;
} ISR_PWM_Data;

// In NRF52, avoid doing something fancy in ISR, for example Serial.print()
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash

void doingSomethingStart(int index);

void doingSomethingStop(int index);

#else   // #if USE_COMPLEX_STRUCT

volatile unsigned long deltaMicrosStart    [NUMBER_ISR_PWMS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
volatile unsigned long previousMicrosStart [NUMBER_ISR_PWMS] = { 0, 0, 0, 0, 0, 0, 0, 0 };

volatile unsigned long deltaMicrosStop     [NUMBER_ISR_PWMS] = { 0, 0, 0, 0, 0, 0, 0, 0 };
volatile unsigned long previousMicrosStop  [NUMBER_ISR_PWMS] = { 0, 0, 0, 0, 0, 0, 0, 0 };

// You can assign pins here. Be carefull to select good pin to use or crash
uint32_t PWM_Pin[NUMBER_ISR_PWMS] =
{
   PIN_D0, PIN_D1, LED_BUILTIN, PIN_D3, PIN_D4, PIN_D5, PIN_D6, PIN_D7
};

// You can assign any interval for any timer here, in microseconds
uint32_t PWM_Period[NUMBER_ISR_PWMS] =
{
  1000000L,   500000L,   333333L,   250000L,   200000L,   166667L,   142857L,   125000L
};


// You can assign any interval for any timer here, in Hz
uint32_t PWM_Freq[NUMBER_ISR_PWMS] =
{
  1,  2,  3,  4,  5,  6,  7,  8
};

// You can assign any interval for any timer here, in Microseconds
uint32_t PWM_DutyCycle[NUMBER_ISR_PWMS] =
{
   5, 10, 20, 30, 40, 45, 50, 55
};

///////////////////////////////////

void doingSomethingStart(int index)
{
  unsigned long currentMicros  = micros();

  deltaMicrosStart[index]    = currentMicros - previousMicrosStart[index];
  previousMicrosStart[index] = currentMicros;
}

void doingSomethingStop(int index)
{
  unsigned long currentMicros  = micros();

  //deltaMicrosStop[index]    = currentMicros - previousMicrosStop[index];
  // Count from start to stop PWM pulse
  deltaMicrosStop[index]    = currentMicros - previousMicrosStart[index];
  previousMicrosStop[index] = currentMicros;
}

#endif    // #if USE_COMPLEX_STRUCT

////////////////////////////////////
// Shared
////////////////////////////////////

void doingSomethingStart0()
{
  doingSomethingStart(0);
}

void doingSomethingStart1()
{
  doingSomethingStart(1);
}

void doingSomethingStart2()
{
  doingSomethingStart(2);
}

void doingSomethingStart3()
{
  doingSomethingStart(3);
}

void doingSomethingStart4()
{
  doingSomethingStart(4);
}

void doingSomethingStart5()
{
  doingSomethingStart(5);
}

void doingSomethingStart6()
{
  doingSomethingStart(6);
}

void doingSomethingStart7()
{
  doingSomethingStart(7);
}

//////////////////////////////////////////////////////

void doingSomethingStop0()
{
  doingSomethingStop(0);
}

void doingSomethingStop1()
{
  doingSomethingStop(1);
}

void doingSomethingStop2()
{
  doingSomethingStop(2);
}

void doingSomethingStop3()
{
  doingSomethingStop(3);
}

void doingSomethingStop4()
{
  doingSomethingStop(4);
}

void doingSomethingStop5()
{
  doingSomethingStop(5);
}

void doingSomethingStop6()
{
  doingSomethingStop(6);
}

void doingSomethingStop7()
{
  doingSomethingStop(7);
}

//////////////////////////////////////////////////////

#if USE_COMPLEX_STRUCT

  #if USING_PWM_FREQUENCY
  
  ISR_PWM_Data curISR_PWM_Data[NUMBER_ISR_PWMS] =
  {
    //pin, irqCallbackStartFunc, irqCallbackStopFunc, PWM_Period, deltaMicrosStart, previousMicrosStart, deltaMicrosStop, previousMicrosStop
    { PIN_D0,       doingSomethingStart0,   doingSomethingStop0,   1,   5, 0, 0, 0, 0 },
    { PIN_D1,       doingSomethingStart1,   doingSomethingStop1,   2,  10, 0, 0, 0, 0 },
    { LED_BUILTIN,  doingSomethingStart2,   doingSomethingStop2,   3,  20, 0, 0, 0, 0 },
    { PIN_D3,       doingSomethingStart3,   doingSomethingStop3,   4,  30, 0, 0, 0, 0 },
    { PIN_D4,       doingSomethingStart4,   doingSomethingStop4,   5,  40, 0, 0, 0, 0 },
    { PIN_D5,       doingSomethingStart5,   doingSomethingStop5,   6,  45, 0, 0, 0, 0 },
    { PIN_D6,       doingSomethingStart6,   doingSomethingStop6,   7,  50, 0, 0, 0, 0 },
    { PIN_D7,       doingSomethingStart7,   doingSomethingStop7,   8,  55, 0, 0, 0, 0 },
  };
  
  #else   // #if USING_PWM_FREQUENCY
  
  ISR_PWM_Data curISR_PWM_Data[NUMBER_ISR_PWMS] =
  {
    //irqCallbackStartFunc, PWM_Period, deltaMicrosStart, previousMicrosStart, deltaMicrosStop, previousMicrosStop
    { PIN_D0,       doingSomethingStart0,   doingSomethingStop0,   1000000L,   5, 0, 0, 0, 0 },
    { PIN_D1,       doingSomethingStart1,   doingSomethingStop1,    500000L,  10, 0, 0, 0, 0 },
    { LED_BUILTIN,  doingSomethingStart2,   doingSomethingStop2,    333333L,  20, 0, 0, 0, 0 },
    { PIN_D3,       doingSomethingStart3,   doingSomethingStop3,    250000L,  30, 0, 0, 0, 0 },
    { PIN_D4,       doingSomethingStart4,   doingSomethingStop4,    200000L,  40, 0, 0, 0, 0 },
    { PIN_D5,       doingSomethingStart5,   doingSomethingStop5,    166667L,  45, 0, 0, 0, 0 },
    { PIN_D6,       doingSomethingStart6,   doingSomethingStop6,    142857L,  50, 0, 0, 0, 0 },
    { PIN_D7,       doingSomethingStart7,   doingSomethingStop7,    125000L,  55, 0, 0, 0, 0 },
  };
  
  #endif  // #if USING_PWM_FREQUENCY

void doingSomethingStart(int index)
{
  unsigned long currentMicros  = micros();

  curISR_PWM_Data[index].deltaMicrosStart    = currentMicros - curISR_PWM_Data[index].previousMicrosStart;
  curISR_PWM_Data[index].previousMicrosStart = currentMicros;
}

void doingSomethingStop(int index)
{
  unsigned long currentMicros  = micros();

  //curISR_PWM_Data[index].deltaMicrosStop     = currentMicros - curISR_PWM_Data[index].previousMicrosStop;
  // Count from start to stop PWM pulse
  curISR_PWM_Data[index].deltaMicrosStop     = currentMicros - curISR_PWM_Data[index].previousMicrosStart;
  curISR_PWM_Data[index].previousMicrosStop  = currentMicros;
}

#else   // #if USE_COMPLEX_STRUCT

irqCallback irqCallbackStartFunc[NUMBER_ISR_PWMS] =
{
  doingSomethingStart0,  doingSomethingStart1,  doingSomethingStart2,  doingSomethingStart3,
  doingSomethingStart4,  doingSomethingStart5,  doingSomethingStart6,  doingSomethingStart7
};

irqCallback irqCallbackStopFunc[NUMBER_ISR_PWMS] =
{
  doingSomethingStop0,  doingSomethingStop1,  doingSomethingStop2,  doingSomethingStop3,
  doingSomethingStop4,  doingSomethingStop5,  doingSomethingStop6,  doingSomethingStop7
};

#endif    // #if USE_COMPLEX_STRUCT

//////////////////////////////////////////////////////

#define SIMPLE_TIMER_MS        2000L

// Init SimpleTimer
SimpleTimer simpleTimer;

// Here is software Timer, you can do somewhat fancy stuffs without many issues.
// But always avoid
// 1. Long delay() it just doing nothing and pain-without-gain wasting CPU power.Plan and design your code / strategy ahead
// 2. Very long "do", "while", "for" loops without predetermined exit time.
void simpleTimerdoingSomething2s()
{
  static unsigned long previousMicrosStart = startMicros;

  unsigned long currMicros = micros();

  Serial.print(F("SimpleTimer (ms): ")); Serial.print(SIMPLE_TIMER_MS);
  Serial.print(F(", ms : ")); Serial.print(currMicros);
  Serial.print(F(", Dms : ")); Serial.println(currMicros - previousMicrosStart);

  for (uint16_t i = 0; i < NUMBER_ISR_PWMS; i++)
  {
#if USE_COMPLEX_STRUCT
    Serial.print(F("PWM Channel : ")); Serial.print(i);
    Serial.print(F(", programmed Period (us): ")); 

  #if USING_PWM_FREQUENCY
    Serial.print(1000000 / curISR_PWM_Data[i].PWM_Freq);
  #else
    Serial.print(curISR_PWM_Data[i].PWM_Period);
  #endif
    
    Serial.print(F(", actual : ")); Serial.print(curISR_PWM_Data[i].deltaMicrosStart);

  Serial.print(F(", programmed DutyCycle : ")); 

  Serial.print(curISR_PWM_Data[i].PWM_DutyCycle);
    
  Serial.print(F(", actual : ")); Serial.println((float) curISR_PWM_Data[i].deltaMicrosStop * 100.0f / curISR_PWM_Data[i].deltaMicrosStart);
    
#else
    Serial.print(F("PWM Channel : ")); Serial.print(i);
    
  #if USING_PWM_FREQUENCY
    Serial.print(1000000 / PWM_Freq[i]);
  #else
    Serial.print(PWM_Period[i]);
  #endif
  
    Serial.print(F(", programmed Period (us): ")); Serial.print(PWM_Period[i]);
    Serial.print(F(", actual : ")); Serial.print(deltaMicrosStart[i]);

    Serial.print(F(", programmed DutyCycle : ")); 
  
    Serial.print(PWM_DutyCycle[i]);
      
    Serial.print(F(", actual : ")); Serial.println( (float) deltaMicrosStop[i] * 100.0f / deltaMicrosStart[i]);
#endif
  }

  previousMicrosStart = currMicros;
}

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(115200);
  while (!Serial);

  delay(2000);

  Serial.print(F("\nStarting ISR_16_PWMs_Array_Complex on ")); Serial.println(ARDUINO_BOARD);
  Serial.println(ESP8266_PWM_VERSION);
  Serial.print(F("CPU Frequency = ")); Serial.print(F_CPU / 1000000); Serial.println(F(" MHz"));

  // Interval in microsecs
  if (ITimer.attachInterruptInterval(HW_TIMER_INTERVAL_US, TimerHandler))
  {
    startMicros = micros();
    Serial.print(F("Starting ITimer OK, micros() = ")); Serial.println(startMicros);
  }
  else
    Serial.println(F("Can't set ITimer. Select another freq. or timer"));

  startMicros = micros();

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_PWM
  
  for (uint16_t i = 0; i < NUMBER_ISR_PWMS; i++)
  {
#if USE_COMPLEX_STRUCT
    curISR_PWM_Data[i].previousMicrosStart = startMicros;
    //ISR_PWM.setInterval(curISR_PWM_Data[i].PWM_Period, curISR_PWM_Data[i].irqCallbackStartFunc);

    //void setPWM(uint32_t pin, uint32_t frequency, uint32_t dutycycle
    // , timer_callback_p StartCallback = nullptr, timer_callback_p StopCallback = nullptr)

  #if USING_PWM_FREQUENCY
    // You can use this with PWM_Freq in Hz
    ISR_PWM.setPWM(curISR_PWM_Data[i].PWM_Pin, curISR_PWM_Data[i].PWM_Freq, curISR_PWM_Data[i].PWM_DutyCycle, 
                   curISR_PWM_Data[i].irqCallbackStartFunc, curISR_PWM_Data[i].irqCallbackStopFunc);
  #else
    // Or You can use this with PWM_Period in us
    ISR_PWM.setPWM_Period(curISR_PWM_Data[i].PWM_Pin, curISR_PWM_Data[i].PWM_Period, curISR_PWM_Data[i].PWM_DutyCycle, 
                          curISR_PWM_Data[i].irqCallbackStartFunc, curISR_PWM_Data[i].irqCallbackStopFunc);
  #endif
  
#else
    previousMicrosStart[i] = micros();
    
  #if USING_PWM_FREQUENCY
    // You can use this with PWM_Freq in Hz
    ISR_PWM.setPWM(PWM_Pin[i], PWM_Freq[i], PWM_DutyCycle[i], irqCallbackStartFunc[i], irqCallbackStopFunc[i]);
  #else
    // Or You can use this with PWM_Period in us
    ISR_PWM.setPWM_Period(PWM_Pin[i], PWM_Period[i], PWM_DutyCycle[i], irqCallbackStartFunc[i], irqCallbackStopFunc[i]);
  #endif 
   
#endif
  }

  // You need this timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary.
  simpleTimer.setInterval(SIMPLE_TIMER_MS, simpleTimerdoingSomething2s);
}

#define BLOCKING_TIME_MS      10000L

void loop()
{
  // This unadvised blocking task is used to demonstrate the blocking effects onto the execution and accuracy to Software timer
  // You see the time elapse of ISR_PWM still accurate, whereas very unaccurate for Software Timer
  // The time elapse for 2000ms software timer now becomes 3000ms (BLOCKING_TIME_MS)
  // While that of ISR_PWM is still prefect.
  delay(BLOCKING_TIME_MS);

  // You need this Software timer for non-critical tasks. Avoid abusing ISR if not absolutely necessary
  // You don't need to and never call ISR_PWM.run() here in the loop(). It's already handled by ISR timer.
  simpleTimer.run();
}
```
---
---

### Debug Terminal Output Samples

### 1. ISR_16_PWMs_Array_Complex on **ESP8266_NODEMCU_ESP12E**

The following is the sample terminal output when running example [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex) to demonstrate the accuracy of ISR Hardware PWM-channels, **especially when system is very busy**.  The ISR PWM-channels is **running exactly according to corresponding programmed periods**


```
Starting ISR_16_PWMs_Array_Complex on ESP8266_NODEMCU_ESP12E
ESP8266_PWM v1.1.0
CPU Frequency = 160 MHz
[PWM] ESP8266TimerInterrupt: _fre = 312500.00 , _count = 6
Starting ITimer OK, micros() = 2068917
Channel : 0	Period : 1000000		OnTime : 50000	Start_Time : 2072412
Channel : 1	Period : 500000		OnTime : 50000	Start_Time : 2072412
Channel : 2	Period : 333333		OnTime : 66666	Start_Time : 2072412
Channel : 3	Period : 250000		OnTime : 75000	Start_Time : 2072412
Channel : 4	Period : 200000		OnTime : 80000	Start_Time : 2072412
Channel : 5	Period : 166666		OnTime : 74999	Start_Time : 2072412
Channel : 6	Period : 142857		OnTime : 71428	Start_Time : 2072412
Channel : 7	Period : 125000		OnTime : 68750	Start_Time : 2072412
SimpleTimer (ms): 2000, ms : 12118352, Dms : 10045971
PWM Channel : 0, programmed Period (us): 1000000, actual : 1000013, programmed DutyCycle : 5, actual : 5.00
PWM Channel : 1, programmed Period (us): 500000, actual : 500008, programmed DutyCycle : 10, actual : 10.00
PWM Channel : 2, programmed Period (us): 333333, actual : 333350, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 3, programmed Period (us): 250000, actual : 250003, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 4, programmed Period (us): 200000, actual : 200006, programmed DutyCycle : 40, actual : 39.99
PWM Channel : 5, programmed Period (us): 166666, actual : 166676, programmed DutyCycle : 45, actual : 44.99
PWM Channel : 6, programmed Period (us): 142857, actual : 142867, programmed DutyCycle : 50, actual : 49.99
PWM Channel : 7, programmed Period (us): 125000, actual : 125011, programmed DutyCycle : 55, actual : 54.98
SimpleTimer (ms): 2000, ms : 22187724, Dms : 10069372
PWM Channel : 0, programmed Period (us): 1000000, actual : 1000013, programmed DutyCycle : 5, actual : 5.00
PWM Channel : 1, programmed Period (us): 500000, actual : 500008, programmed DutyCycle : 10, actual : 10.00
PWM Channel : 2, programmed Period (us): 333333, actual : 333350, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 3, programmed Period (us): 250000, actual : 250003, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 4, programmed Period (us): 200000, actual : 200006, programmed DutyCycle : 40, actual : 39.99
PWM Channel : 5, programmed Period (us): 166666, actual : 166676, programmed DutyCycle : 45, actual : 44.99
PWM Channel : 6, programmed Period (us): 142857, actual : 142868, programmed DutyCycle : 50, actual : 49.99
PWM Channel : 7, programmed Period (us): 125000, actual : 125012, programmed DutyCycle : 55, actual : 54.98
SimpleTimer (ms): 2000, ms : 32257074, Dms : 10069350
PWM Channel : 0, programmed Period (us): 1000000, actual : 1000013, programmed DutyCycle : 5, actual : 5.00
PWM Channel : 1, programmed Period (us): 500000, actual : 500008, programmed DutyCycle : 10, actual : 10.00
PWM Channel : 2, programmed Period (us): 333333, actual : 333350, programmed DutyCycle : 20, actual : 20.00
PWM Channel : 3, programmed Period (us): 250000, actual : 250003, programmed DutyCycle : 30, actual : 30.00
PWM Channel : 4, programmed Period (us): 200000, actual : 200007, programmed DutyCycle : 40, actual : 39.99
PWM Channel : 5, programmed Period (us): 166666, actual : 166674, programmed DutyCycle : 45, actual : 44.99
PWM Channel : 6, programmed Period (us): 142857, actual : 142868, programmed DutyCycle : 50, actual : 49.99
PWM Channel : 7, programmed Period (us): 125000, actual : 125012, programmed DutyCycle : 55, actual : 54.98
```

---

### 2. ISR_16_PWMs_Array on **ESP8266_NODEMCU_ESP12E**

The following is the sample terminal output when running example [ISR_16_PWMs_Array](examples/ISR_16_PWMs_Array) to demonstrate how to use multiple Hardware PWM channels.

```
Starting ISR_16_PWMs_Array on ESP8266_NODEMCU_ESP12E
ESP8266_PWM v1.1.0
CPU Frequency = 160 MHz
[PWM] ESP8266TimerInterrupt: _fre = 312500.00 , _count = 6
Starting ITimer OK, micros() = 2069342
Channel : 0	Period : 1000000		OnTime : 50000	Start_Time : 2072822
Channel : 1	Period : 500000		OnTime : 50000	Start_Time : 2072822
Channel : 2	Period : 333333		OnTime : 66666	Start_Time : 2072822
Channel : 3	Period : 250000		OnTime : 75000	Start_Time : 2072822
Channel : 4	Period : 200000		OnTime : 80000	Start_Time : 2072822
Channel : 5	Period : 166667		OnTime : 75000	Start_Time : 2072822
Channel : 6	Period : 142857		OnTime : 71428	Start_Time : 2072822
Channel : 7	Period : 125000		OnTime : 68750	Start_Time : 2072822
```

---


### 3. ISR_16_PWMs_Array_Simple on **ESP8266_NODEMCU_ESP12E**

The following is the sample terminal output when running example [ISR_16_PWMs_Array_Simple](examples/ISR_16_PWMs_Array_Simple) to demonstrate how to use multiple Hardware PWM channels.

```
Starting ISR_16_PWMs_Array_Simple on ESP8266_NODEMCU_ESP12E
ESP8266_PWM v1.1.0
CPU Frequency = 160 MHz
[PWM] ESP8266TimerInterrupt: _fre = 312500.00 , _count = 6
Starting ITimer OK, micros() = 2069144
Channel : 0	Period : 1000000		OnTime : 50000	Start_Time : 2072624
Channel : 1	Period : 500000		OnTime : 50000	Start_Time : 2072624
Channel : 2	Period : 333333		OnTime : 66666	Start_Time : 2072624
Channel : 3	Period : 250000		OnTime : 75000	Start_Time : 2072624
Channel : 4	Period : 200000		OnTime : 80000	Start_Time : 2072624
Channel : 5	Period : 166667		OnTime : 75000	Start_Time : 2072624
Channel : 6	Period : 142857		OnTime : 71428	Start_Time : 2072624
Channel : 7	Period : 125000		OnTime : 68750	Start_Time : 2072624
```

---

### 4. ISR_Modify_PWM on **ESP8266_NODEMCU_ESP12E**

The following is the sample terminal output when running example [ISR_Modify_PWM](examples/ISR_Modify_PWM) to demonstrate how to modify PWM settings on-the-fly without deleting the PWM channel

```
Starting ISR_Modify_PWM on ESP8266_NODEMCU_ESP12E
ESP8266_PWM v1.1.0
CPU Frequency = 160 MHz
[PWM] ESP8266TimerInterrupt: _fre = 312500.00 , _count = 6
Starting ITimer OK, micros() = 2067012
Using PWM Freq = 1.00, PWM DutyCycle = 10
Channel : 0	Period : 1000000		OnTime : 100000	Start_Time : 2074213
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 12081063
Channel : 0	Period : 1000000		OnTime : 100000	Start_Time : 22082014
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 32083015
Channel : 0	Period : 1000000		OnTime : 100000	Start_Time : 42084018
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 52085016
Channel : 0	Period : 1000000		OnTime : 100000	Start_Time : 62086011
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 72087015
Channel : 0	Period : 1000000		OnTime : 100000	Start_Time : 82088012
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 92089018
Channel : 0	Period : 1000000		OnTime : 100000	Start_Time : 102090015
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 112091018
Channel : 0	Period : 1000000		OnTime : 100000	Start_Time : 122092014
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 132093012
```

---

### 5. ISR_Changing_PWM on **ESP8266_NODEMCU_ESP12E**

The following is the sample terminal output when running example [ISR_Changing_PWM](examples/ISR_Changing_PWM) to demonstrate how to modify PWM settings on-the-fly by deleting the PWM channel and reinit the PWM channel

```
Starting ISR_Changing_PWM on ESP8266_NODEMCU_ESP12E
ESP8266_PWM v1.1.0
CPU Frequency = 160 MHz
[PWM] ESP8266TimerInterrupt: _fre = 312500.00 , _count = 6
Starting ITimer OK, micros() = 2069740
Using PWM Freq = 1.00, PWM DutyCycle = 50
Channel : 0	Period : 1000000		OnTime : 500000	Start_Time : 2077044
Using PWM Freq = 2.00, PWM DutyCycle = 90
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 12082957
Using PWM Freq = 1.00, PWM DutyCycle = 50
Channel : 0	Period : 1000000		OnTime : 500000	Start_Time : 22083187
Using PWM Freq = 2.00, PWM DutyCycle = 90
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 32083386
Using PWM Freq = 1.00, PWM DutyCycle = 50
Channel : 0	Period : 1000000		OnTime : 500000	Start_Time : 42083596
Using PWM Freq = 2.00, PWM DutyCycle = 90
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 52083796
Using PWM Freq = 1.00, PWM DutyCycle = 50
Channel : 0	Period : 1000000		OnTime : 500000	Start_Time : 62084005
Using PWM Freq = 2.00, PWM DutyCycle = 90
Channel : 0	Period : 500000		OnTime : 450000	Start_Time : 72084206
```

---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level `_PWM_LOGLEVEL_` from 0 to 4

```cpp
// These define's must be placed at the beginning before #include "ESP8266_PWM.h"
// _PWM_LOGLEVEL_ from 0 to 4
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_      4
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [ESP8266_PWM issues](https://github.com/khoih-prog/ESP8266_PWM/issues)

---

## TO DO

1. Search for bug and improvement.
2. Similar features for remaining Arduino boards such as SAMD21, SAMD51, SAM-DUE, nRF52, STM32, Portenta_H7, RP2040, etc.



## DONE

1. Basic hardware PWM-channels for ESP8266 for [ESP8266 core v3.0.2+](https://github.com/esp8266/Arduino/releases/tag/3.0.2)
2. Longer time interval
3. Add complex examples.
4. Add functions to modify PWM settings on-the-fly

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.

1. Thanks to [AnselPeng2019](https://github.com/AnselPeng2019) to request new feature to modify PWM settings on-the-fly in

- [Change Duty Cycle #1](https://github.com/khoih-prog/ESP8266_PWM/issues/1) leading to v1.1.0


<table>
  <tr>
    <td align="center"><a href="https://github.com/AnselPeng2019"><img src="https://github.com/AnselPeng2019.png" width="100px;" alt="AnselPeng2019"/><br /><sub><b>AnselPeng2019</b></sub></a><br /></td>
  </tr>
</table>

---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---

### License

- The library is licensed under [MIT](https://github.com/khoih-prog/ESP8266_PWM/blob/master/LICENSE)

---

## Copyright

Copyright 2021- Khoi Hoang


