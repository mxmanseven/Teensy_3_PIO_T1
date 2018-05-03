#include <Arduino.h>
#include "display.h"
#include "WheelManager.h"
#include "SpeedInterval.h"
//#include <LiquidCrystal.h>

//#include <Wire.h> //https://forum.pjrc.com/threads/21680-New-I2C-library-for-Teensy3
#include <i2c_t3.h>
#include <hd44780.h>                       // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h> // i2c expander i/o class header

#include <extEEPROM.h>    //http://github.com/JChristensen/extEEPROM/tree/dev

#include "TimeService.h"

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip

TimeServicKnh timeKnh;

const uint32_t totalKBytes = 4;         //for read and write test functions
byte eepromI2cAddr = 0x50;
extEEPROM eep(kbits_32, 1, 32, eepromI2cAddr);         //device size, number of devices, page size

void setup() 
{
    #if WHEEL_MANAGER_DEBUG == 1
    wmTest();
    #endif

    Serial.begin(9600);

    Wire.begin(
        I2C_MASTER, 
        0x00, 
        I2C_PINS_16_17, 
        I2C_PULLUP_EXT, 
        400000);

    eep.begin(extEEPROM::twiClock400kHz);

    // write
    delay(100);
    eep.write(0, 13);

    // read
}

void loop() {
    delay(1000);


    int readValue = 0;
    readValue = eep.read(0);
    Serial.println(String(readValue));


    // int unixTime = timeKnh.getUnitxTime();
    // Serial.println(String(unixTime)); 
}