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

    while(!Serial) {}

    Wire.begin(
        I2C_MASTER, 
        0x00, 
        I2C_PINS_16_17, 
        I2C_PULLUP_EXT, 
        400000);

    //Wire.begin();

    // disable internal pull up which is set by default in Wire.h
    Serial.println("sda: " +  String(SDA));
    Serial.println("sdl: " +  String(SCL));


    // digitalWrite(18, 1);
    // digitalWrite(19, 1);

    delay(100);

// write
    eep.begin(extEEPROM::twiClock400kHz);
    eep.write(10, 13);

    // read
}

void i2c_eeprom_write_byte( int deviceaddress, unsigned int eeaddress, byte data ) {
    int rdata = data;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
}

byte i2c_eeprom_read_byte( int deviceaddress, unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(deviceaddress,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
}


// 0x57 works!
int deviceaAddr = 0;

void loop() {
    delay(500);

    Serial.println("deviceAddr: " + String(deviceaAddr));

    i2c_eeprom_write_byte(deviceaAddr, 0, 12);
    delay(100);

    int readValue = 0;
    readValue = i2c_eeprom_read_byte(deviceaAddr, 0);
    Serial.println("eeprom " + String(readValue));

    if(readValue != 0xff)
    {
        Serial.println("i2c worked addr:" + String(deviceaAddr));
        while(1) {}
    }

    // int readValue = 0;
    // readValue = eep.read(10);
    // Serial.println("eeprom " + String(readValue));

    int unixTime = timeKnh.getUnitxTime();
    Serial.println(String(unixTime)); 

    deviceaAddr++;
}