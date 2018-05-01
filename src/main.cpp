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

hd44780_I2Cexp lcd; // declare lcd object: auto locate & config exapander chip

TimeServicKnh timeKnh;


void setup() 
{
    #if WHEEL_MANAGER_DEBUG == 1
    wmTest();
    #endif


    //https://www.amazon.com/Holdding-AT24C32-Precision-Temperature-compensated-Oscillator/dp/B00LZCTMJM/ref=pd_sbs_147_1?_encoding=UTF8&pd_rd_i=B00LZCTMJM&pd_rd_r=A9K9RE6N0RQW3TP137PY&pd_rd_w=yF4cX&pd_rd_wg=9EIIm&psc=1&refRID=A9K9RE6N0RQW3TP137PY



    Serial.begin(9600);

    // eeprom
    // slave device address is 0xA0 write
    // and 0xA1 read for eeprom at24c32
    // ds 3231 rtu i2c addr
    Wire.begin(
        I2C_MASTER, 
        0x00, 
        I2C_PINS_16_17, 
        I2C_PULLUP_INT , 
        400000);

        // //Clock.setClockMode(false);	// set to 24h
		// Clock.setClockMode(true);	// set to 12h

}

void loop() {
    delay(1000);

    int unixTime = timeKnh.getUnitxTime();
    Serial.println(String(unixTime));
  
}

