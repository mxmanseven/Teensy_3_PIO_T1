// #include <Arduino.h>

// void setup() {
//   Serial.begin(9600);
// }

// void loop() {
//   Serial.println("hi");
//   delay(2000);
// }




#include <Arduino.h>
#include "display.h"
#include "WheelManager.h"
#include "SpeedInterval.h"
//#include <LiquidCrystal.h>


//#include <Wire.h> //https://forum.pjrc.com/threads/21680-New-I2C-library-for-Teensy3
#include <i2c_t3.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip

void setup() 
{
    #if WHEEL_MANAGER_DEBUG == 1
    wmTest();
    #endif
}

void loop() {
}

