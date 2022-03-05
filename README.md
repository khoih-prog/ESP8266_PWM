# ESP8266_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP8266_PWM.svg?)](https://www.ardu-badge.com/ESP8266_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP8266_PWM.svg)](https://github.com/khoih-prog/ESP8266_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP8266_PWM/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP8266_PWM.svg)](http://github.com/khoih-prog/ESP8266_PWM/issues)

<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of Contents

* [Important Change from v1.2.0](#Important-Change-from-v120)
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
  * [ 6. multiFileProject](examples/multiFileProject)
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

### Important Change from v1.2.0

Please have a look at [HOWTO Fix `Multiple Definitions` Linker Error](#howto-fix-multiple-definitions-linker-error)

As more complex calculation and check **inside ISR** are introduced from v1.2.0, there is possibly some crash depending on use-case.

You can modify to use larger `HW_TIMER_INTERVAL_US`, (from current 20uS), according to your board and use-case if crash happens.


```
// Current 20uS
#define HW_TIMER_INTERVAL_US      20L
```

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

1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
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
3. Install [**ESP8266_PWM** library](https://registry.platformio.org/libraries/khoih-prog/ESP8266_PWM) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/ESP8266_PWM/installation). Search for **ESP8266_PWM** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)


---
---


### HOWTO Fix `Multiple Definitions` Linker Error

The current library implementation, using `xyz-Impl.h` instead of standard `xyz.cpp`, possibly creates certain `Multiple Definitions` Linker error in certain use cases.

You can include this `.hpp` file

```
// Can be included as many times as necessary, without `Multiple Definitions` Linker Error
#include "ESP8266_PWM.hpp"     //https://github.com/khoih-prog/ESP8266_PWM
```

in many files. But be sure to use the following `.h` file **in just 1 `.h`, `.cpp` or `.ino` file**, which must **not be included in any other file**, to avoid `Multiple Definitions` Linker Error

```
// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ESP8266_PWM.h"           //https://github.com/khoih-prog/ESP8266_PWM
```

Check the new [**multiFileProject** example](examples/multiFileProject) for a `HOWTO` demo.

Have a look at the discussion in [Different behaviour using the src_cpp or src_h lib #80](https://github.com/khoih-prog/ESPAsync_WiFiManager/discussions/80)


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
 6. [**multiFileProject**](examples/multiFileProject) **New** 


---
---

### Example [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex)

https://github.com/khoih-prog/ESP8266_PWM/blob/192da873e974795080a2e38163ed4397c20f1050/examples/ISR_16_PWMs_Array_Complex/ISR_16_PWMs_Array_Complex.ino#L24-L481


---
---

### Debug Terminal Output Samples

### 1. ISR_16_PWMs_Array_Complex on **ESP8266_NODEMCU_ESP12E**

The following is the sample terminal output when running example [ISR_16_PWMs_Array_Complex](examples/ISR_16_PWMs_Array_Complex) to demonstrate the accuracy of ISR Hardware PWM-channels, **especially when system is very busy**.  The ISR PWM-channels is **running exactly according to corresponding programmed periods**


```
Starting ISR_16_PWMs_Array_Complex on ESP8266_NODEMCU_ESP12E
ESP8266_PWM v1.2.4
CPU Frequency = 160 MHz
[PWM] ESP8266TimerInterrupt: Timer Clock fre = 80000000
[PWM] Timer fre = 50000.00 , _count = 1600
Starting ITimer OK, micros() = 2073808
Channel : 0	    Period : 1000000		OnTime : 50000	Start_Time : 2077435
Channel : 1	    Period : 500000		OnTime : 50000	Start_Time : 2083470
Channel : 2	    Period : 333333		OnTime : 66666	Start_Time : 2089512
Channel : 3	    Period : 250000		OnTime : 75000	Start_Time : 2095588
Channel : 4	    Period : 200000		OnTime : 80000	Start_Time : 2101657
Channel : 5	    Period : 166666		OnTime : 74999	Start_Time : 2107738
Channel : 6	    Period : 142857		OnTime : 71428	Start_Time : 2113805
Channel : 7	    Period : 125000		OnTime : 68750	Start_Time : 2119875
SimpleTimer (ms): 2000, ms : 12126030, Dms : 10048756
PWM Channel : 0, programmed Period (us): 1000000.00, actual : 1000005, programmed DutyCycle : 5.00, actual : 5.00
PWM Channel : 1, programmed Period (us): 500000.00, actual : 500013, programmed DutyCycle : 10.00, actual : 10.00
PWM Channel : 2, programmed Period (us): 333333.34, actual : 333348, programmed DutyCycle : 20.00, actual : 20.00
PWM Channel : 3, programmed Period (us): 250000.00, actual : 250016, programmed DutyCycle : 30.00, actual : 29.99
PWM Channel : 4, programmed Period (us): 200000.00, actual : 200004, programmed DutyCycle : 40.00, actual : 39.99
PWM Channel : 5, programmed Period (us): 166666.67, actual : 166684, programmed DutyCycle : 45.00, actual : 44.99
PWM Channel : 6, programmed Period (us): 142857.14, actual : 142869, programmed DutyCycle : 50.00, actual : 49.99
PWM Channel : 7, programmed Period (us): 125000.00, actual : 125018, programmed DutyCycle : 55.00, actual : 54.99
SimpleTimer (ms): 2000, ms : 22199583, Dms : 10073553
PWM Channel : 0, programmed Period (us): 1000000.00, actual : 1000005, programmed DutyCycle : 5.00, actual : 5.00
PWM Channel : 1, programmed Period (us): 500000.00, actual : 500012, programmed DutyCycle : 10.00, actual : 10.00
PWM Channel : 2, programmed Period (us): 333333.34, actual : 333348, programmed DutyCycle : 20.00, actual : 20.00
PWM Channel : 3, programmed Period (us): 250000.00, actual : 250016, programmed DutyCycle : 30.00, actual : 29.99
PWM Channel : 4, programmed Period (us): 200000.00, actual : 200005, programmed DutyCycle : 40.00, actual : 39.99
PWM Channel : 5, programmed Period (us): 166666.67, actual : 166684, programmed DutyCycle : 45.00, actual : 44.99
PWM Channel : 6, programmed Period (us): 142857.14, actual : 142870, programmed DutyCycle : 50.00, actual : 49.99
PWM Channel : 7, programmed Period (us): 125000.00, actual : 125019, programmed DutyCycle : 55.00, actual : 54.99
SimpleTimer (ms): 2000, ms : 32273095, Dms : 10073512
PWM Channel : 0, programmed Period (us): 1000000.00, actual : 1000004, programmed DutyCycle : 5.00, actual : 5.00
PWM Channel : 1, programmed Period (us): 500000.00, actual : 500012, programmed DutyCycle : 10.00, actual : 10.00
PWM Channel : 2, programmed Period (us): 333333.34, actual : 333348, programmed DutyCycle : 20.00, actual : 20.00
PWM Channel : 3, programmed Period (us): 250000.00, actual : 250016, programmed DutyCycle : 30.00, actual : 29.99
PWM Channel : 4, programmed Period (us): 200000.00, actual : 200005, programmed DutyCycle : 40.00, actual : 39.99
PWM Channel : 5, programmed Period (us): 166666.67, actual : 166685, programmed DutyCycle : 45.00, actual : 44.99
PWM Channel : 6, programmed Period (us): 142857.14, actual : 142870, programmed DutyCycle : 50.00, actual : 49.99
PWM Channel : 7, programmed Period (us): 125000.00, actual : 125019, programmed DutyCycle : 55.00, actual : 54.99
```

---

### 2. ISR_16_PWMs_Array on **ESP8266_NODEMCU_ESP12E**

The following is the sample terminal output when running example [ISR_16_PWMs_Array](examples/ISR_16_PWMs_Array) to demonstrate how to use multiple Hardware PWM channels.

```
Starting ISR_16_PWMs_Array on ESP8266_NODEMCU_ESP12E
ESP8266_PWM v1.2.4
CPU Frequency = 160 MHz
[PWM] ESP8266TimerInterrupt: Timer Clock fre = 80000000
[PWM] Timer fre = 50000.00 , _count = 1600
Starting ITimer OK, micros() = 2072589
Channel : 0	    Period : 1000000	OnTime : 50000	Start_Time : 2076168
Channel : 1	    Period : 500000		OnTime : 50000	Start_Time : 2082485
Channel : 2	    Period : 333333		OnTime : 66666	Start_Time : 2088818
Channel : 3	    Period : 250000		OnTime : 75000	Start_Time : 2095149
Channel : 4	    Period : 200000		OnTime : 80000	Start_Time : 2101479
Channel : 5	    Period : 166667		OnTime : 75000	Start_Time : 2107820
Channel : 6	    Period : 142857		OnTime : 71428	Start_Time : 2114152
Channel : 7	    Period : 125000		OnTime : 68750	Start_Time : 2120482
```

---


### 3. ISR_16_PWMs_Array_Simple on **ESP8266_NODEMCU_ESP12E**

The following is the sample terminal output when running example [ISR_16_PWMs_Array_Simple](examples/ISR_16_PWMs_Array_Simple) to demonstrate how to use multiple Hardware PWM channels.

```
Starting ISR_16_PWMs_Array_Simple on ESP8266_NODEMCU_ESP12E
ESP8266_PWM v1.2.4
CPU Frequency = 160 MHz
[PWM] ESP8266TimerInterrupt: Timer Clock fre = 80000000
[PWM] Timer fre = 50000.00 , _count = 1600
Starting ITimer OK, micros() = 2073472
Channel : 0	    Period : 1000000	OnTime : 50000	Start_Time : 2077055
Channel : 1	    Period : 500000		OnTime : 50000	Start_Time : 2083379
Channel : 2	    Period : 333333		OnTime : 66666	Start_Time : 2089704
Channel : 3	    Period : 250000		OnTime : 75000	Start_Time : 2096036
Channel : 4	    Period : 200000		OnTime : 80000	Start_Time : 2102366
Channel : 5	    Period : 166667		OnTime : 75000	Start_Time : 2108707
Channel : 6	    Period : 142857		OnTime : 71428	Start_Time : 2115038
Channel : 7	    Period : 125000		OnTime : 68750	Start_Time : 2121369
```

---

### 4. ISR_Modify_PWM on **ESP8266_NODEMCU_ESP12E**

The following is the sample terminal output when running example [ISR_Modify_PWM](examples/ISR_Modify_PWM) to demonstrate how to modify PWM settings on-the-fly without deleting the PWM channel

```
Starting ISR_Modify_PWM on ESP8266_NODEMCU_ESP12E
ESP8266_PWM v1.2.4
CPU Frequency = 160 MHz
[PWM] ESP8266TimerInterrupt: Timer Clock fre = 80000000
[PWM] Timer fre = 50000.00 , _count = 1600
Starting ITimer OK, micros() = 2072681
Using PWM Freq = 200.00, PWM DutyCycle = 1.00
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 2080334
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 12082106
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 22083353
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 32084600
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 42085847
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 52087094
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 62088341
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 72089588
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 82090835
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 92092082
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 102093328
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 112094575
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 122095822
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 132097069
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 142098316
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 152099563
Channel : 0	    Period : 5000		OnTime : 50	Start_Time : 162100810
Channel : 0	    Period : 10000		OnTime : 555	Start_Time : 172102056
```

---

### 5. ISR_Changing_PWM on **ESP8266_NODEMCU_ESP12E**

The following is the sample terminal output when running example [ISR_Changing_PWM](examples/ISR_Changing_PWM) to demonstrate how to modify PWM settings on-the-fly by deleting the PWM channel and reinit the PWM channel

```
Starting ISR_Changing_PWM on ESP8266_NODEMCU_ESP12E
ESP8266_PWM v1.2.4
CPU Frequency = 160 MHz
[PWM] ESP8266TimerInterrupt: Timer Clock fre = 80000000
[PWM] Timer fre = 50000.00 , _count = 1600
Starting ITimer OK, micros() = 2073309
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Channel : 0	    Period : 1000000		OnTime : 500000	Start_Time : 2080902
Using PWM Freq = 2.00, PWM DutyCycle = 90.00
Channel : 0	    Period : 500000		OnTime : 450000	Start_Time : 12087445
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Channel : 0	    Period : 1000000		OnTime : 500000	Start_Time : 22087762
Using PWM Freq = 2.00, PWM DutyCycle = 90.00
Channel : 0	    Period : 500000		OnTime : 450000	Start_Time : 32088065
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Channel : 0	    Period : 1000000		OnTime : 500000	Start_Time : 42088370
Using PWM Freq = 2.00, PWM DutyCycle = 90.00
Channel : 0	    Period : 500000		OnTime : 450000	Start_Time : 52088673
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Channel : 0	    Period : 1000000		OnTime : 500000	Start_Time : 62088982
Using PWM Freq = 2.00, PWM DutyCycle = 90.00
Channel : 0	    Period : 500000		OnTime : 450000	Start_Time : 72089285
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Channel : 0	    Period : 1000000		OnTime : 500000	Start_Time : 82089593
Using PWM Freq = 2.00, PWM DutyCycle = 90.00
Channel : 0	    Period : 500000		OnTime : 450000	Start_Time : 92089898
Using PWM Freq = 1.00, PWM DutyCycle = 50.00
Channel : 0	    Period : 1000000		OnTime : 500000	Start_Time : 102090204
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
5. Improve accuracy by using `float`, instead of `uint32_t` for `dutycycle`
6. Optimize library code by using `reference-passing` instead of `value-passing`
7. DutyCycle to be optionally updated at the end current PWM period instead of immediately.
8. Display informational warning only when `_PWM_LOGLEVEL_` > 3

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.

1. Thanks to [AnselPeng2019](https://github.com/AnselPeng2019) to request new feature to modify PWM settings on-the-fly in

- [Change Duty Cycle #1](https://github.com/khoih-prog/ESP8266_PWM/issues/1) leading to v1.1.0

2. Thanks to [ggmichael](https://github.com/ggmichael) to request better PWM resolution, leading to v1.2.0, in

- [Change Duty Cycle #1](https://github.com/khoih-prog/ESP8266_PWM/issues/1#issuecomment-1024969658)

and enhancement request as well as testing to verify, leading to v1.2.2, in

- [DutyCycle to be updated at the end current PWM period #2](https://github.com/khoih-prog/ESP8266_PWM/issues/2)


<table>
  <tr>
    <td align="center"><a href="https://github.com/AnselPeng2019"><img src="https://github.com/AnselPeng2019.png" width="100px;" alt="AnselPeng2019"/><br /><sub><b>AnselPeng2019</b></sub></a><br /></td>
    <td align="center"><a href="https://github.com/ggmichael"><img src="https://github.com/ggmichael.png" width="100px;" alt="ggmichael"/><br /><sub><b>ggmichael</b></sub></a><br /></td>
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


