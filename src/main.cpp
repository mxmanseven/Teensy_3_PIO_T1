#include <Arduino.h>
#include "display.h"
#include "WheelManager.h"
#include "SpeedInterval.h"
//#include <LiquidCrystal.h>

//#include <Wire.h> //https://forum.pjrc.com/threads/21680-New-I2C-library-for-Teensy3
#include <i2c_t3.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

#include "TimeService.h"
#include "EepromIic.h"
#include "Route.h"
#include "EnduroManager.h"

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip

TimeServicKnh timeKnh;

EepromIic eepromIic;

void setup() 
{
    Serial.begin(57600);

    while(!Serial) {}

    Wire.begin(
        I2C_MASTER, 
        0x00, 
        I2C_PINS_16_17, 
        I2C_PULLUP_EXT, 
        400000);

    delay(100);

    #if WHEEL_MANAGER_DEBUG == 1
    wmTest();
    #endif

    #if ROUTE_DEBUG == 1
    RouteTest();
    #endif

    //EnduroManagerTest();
}

void loop() {
     delay(1000);

    // eepromIic.write_byte(0, 27);
    // delay(100);

    // int readValue = 0;
    // readValue = eepromIic.read_byte(0);
    // Serial.println("eeprom " + String(readValue));

    // int unixTime = timeKnh.getUnitxTime();
    // Serial.println(String(unixTime)); 
}