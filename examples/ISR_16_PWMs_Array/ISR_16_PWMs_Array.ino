/****************************************************************************************************************************
  ISR_16_PWMs_Array.ino
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
*****************************************************************************************************************************/

#if !defined(ESP8266)
  #error This code is designed to run on ESP8266 and ESP8266-based boards! Please check your Tools->Board setting.
#endif

// These define's must be placed at the beginning before #include "ESP8266_PWM.h"
// _PWM_LOGLEVEL_ from 0 to 4
// Don't define _PWM_LOGLEVEL_ > 0. Only for special ISR debugging only. Can hang the system.
#define _PWM_LOGLEVEL_                3

#define USING_MICROS_RESOLUTION       true    //false

// Default is true, uncomment to false
//#define CHANGING_PWM_END_OF_CYCLE     false

// Select a Timer Clock
#define USING_TIM_DIV1                true              // for shortest and most accurate timer
#define USING_TIM_DIV16               false             // for medium time and medium accurate timer
#define USING_TIM_DIV256              false             // for longest timer but least accurate. Default

// To be included only in main(), .ino with setup() to avoid `Multiple Definitions` Linker Error
#include "ESP8266_PWM.h"

#ifndef LED_BUILTIN
  #define LED_BUILTIN       2
#endif

#define HW_TIMER_INTERVAL_US      20L

uint32_t startMicros = 0;

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

//////////////////////////////////////////////////////

#define USING_PWM_FREQUENCY     true

//////////////////////////////////////////////////////

// You can assign pins here. Be carefull to select good pin to use or crash
uint32_t PWM_Pin[] =
{
  PIN_D0, PIN_D1, LED_BUILTIN, PIN_D3, PIN_D4, PIN_D5, PIN_D6, PIN_D7
};

// You can assign any interval for any timer here, in microseconds
uint32_t PWM_Period[] =
{
  1000000,   500000,   333333,   250000,   200000,   166667,   142857,   125000
};


// You can assign any interval for any timer here, in Hz
float PWM_Freq[] =
{
  1.0,  2.0,  3.0,  4.0,  5.0,  6.0,  7.0,  8.0
};

// You can assign any interval for any timer here, in Microseconds
float PWM_DutyCycle[] =
{
   5.0, 10.0, 20.0, 30.0, 40.0, 45.0, 50.0, 55.0
};


typedef void (*irqCallback)  ();


// In ESP8266, avoid doing something fancy in ISR, for example complex Serial.print with String() argument
// The pure simple Serial.prints here are just for demonstration and testing. Must be eliminate in working environment
// Or you can get this run-time error / crash
void doingSomething0()
{
}

void doingSomething1()
{
}

void doingSomething2()
{
}

void doingSomething3()
{
}

void doingSomething4()
{
}

void doingSomething5()
{
}

void doingSomething6()
{
}

void doingSomething7()
{
}


irqCallback irqCallbackStartFunc[] =
{
  doingSomething0,  doingSomething1,  doingSomething2,  doingSomething3, 
  doingSomething4,  doingSomething5,  doingSomething6,  doingSomething7
};

////////////////////////////////////////////////

void setup()
{
  pinMode(PIN_D1, OUTPUT);
  
  Serial.begin(115200);
  while (!Serial);

  delay(2000);

  Serial.print(F("\nStarting ISR_16_PWMs_Array on ")); Serial.println(ARDUINO_BOARD);
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

  // Just to demonstrate, don't use too many ISR Timers if not absolutely necessary
  // You can use up to 16 timer for each ISR_PWM
  for (uint16_t i = 0; i < NUMBER_ISR_PWMS; i++)
  {
    //void setPWM(uint32_t pin, float frequency, float dutycycle
    // , timer_callback_p StartCallback = nullptr, timer_callback_p StopCallback = nullptr)

#if USING_PWM_FREQUENCY

    // You can use this with PWM_Freq in Hz
    ISR_PWM.setPWM(PWM_Pin[i], PWM_Freq[i], PWM_DutyCycle[i], irqCallbackStartFunc[i]);

#else
  #if USING_MICROS_RESOLUTION
    // Or using period in microsecs resolution
    ISR_PWM.setPWM_Period(PWM_Pin[i], PWM_Period[i], PWM_DutyCycle[i], irqCallbackStartFunc[i]);
  #else
    // Or using period in millisecs resolution
    ISR_PWM.setPWM_Period(PWM_Pin[i], PWM_Period[i] / 1000, PWM_DutyCycle[i], irqCallbackStartFunc[i]);
  #endif
#endif    
  }
}

void loop()
{
}
