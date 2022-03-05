# ESP8266_PWM Library

[![arduino-library-badge](https://www.ardu-badge.com/badge/ESP8266_PWM.svg?)](https://www.ardu-badge.com/ESP8266_PWM)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/ESP8266_PWM.svg)](https://github.com/khoih-prog/ESP8266_PWM/releases)
[![GitHub](https://img.shields.io/github/license/mashape/apistatus.svg)](https://github.com/khoih-prog/ESP8266_PWM/blob/master/LICENSE)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/ESP8266_PWM.svg)](http://github.com/khoih-prog/ESP8266_PWM/issues)

---
---

## Table of Contents

* [Changelog](#changelog)
  * [Releases v1.2.4](#releases-v124)
  * [Releases v1.2.3](#releases-v123)
  * [Releases v1.2.2](#releases-v122)
  * [Releases v1.2.1](#releases-v121)
  * [Releases v1.2.0](#releases-v120)
  * [Releases v1.1.0](#releases-v110)
  * [Releases v1.0.0](#releases-v100)

---
---

## Changelog

### Releases v1.2.4

1. Fix `DutyCycle` bug. Check [float precisison of DutyCycle only sometimes working #3](https://github.com/khoih-prog/SAMD_Slow_PWM/issues/3)
2. Fix `New Period` display bug. Check [random dropouts #4](https://github.com/khoih-prog/SAMD_Slow_PWM/issues/4)
3. Update examples

### Releases v1.2.3

1. Use `float` for `DutyCycle` and `Freq`, `uint32_t` for `period`. 
2. Optimize code by not calculation in ISR

### Releases v1.2.2

1. DutyCycle to be optionally updated at the end current PWM period instead of immediately. Check [DutyCycle to be updated at the end current PWM period #2](https://github.com/khoih-prog/ESP8266_PWM/issues/2)


### Releases v1.2.1

1. Fix bug.
2. Optimize library code by using `reference-passing` instead of `value-passing`

### Releases v1.2.0

1. Fix `multiple-definitions` linker error. Drop `src_cpp` and `src_h` directories
2. Add feature to select among highest, medium or lowest accuracy for Timers for shortest, medium or longest time
3. Fix reattachInterrupt() bug. Check [bugfix: reattachInterrupt() pass wrong frequency value to setFrequency() #19](https://github.com/khoih-prog/ESP8266TimerInterrupt/pull/19)
4. Add example [multiFileProject](examples/multiFileProject) to demo for multiple-file project
5. Improve accuracy by using `double`, instead of `uint32_t` for `dutycycle`, `period`. Check [Change Duty Cycle #1](https://github.com/khoih-prog/ESP8266_PWM/issues/1#issuecomment-1024969658)
6. Update examples accordingly

### Releases v1.1.0

1. Add functions to modify PWM settings on-the-fly
2. Fix bug
3. Add example to demo how to modify PWM settings on-the-fly

### Releases v1.0.0

1. Initial coding for ESP8266 boards using [ESP8266 core v3.0.2+](https://github.com/esp8266/Arduino/releases/tag/3.0.2)


