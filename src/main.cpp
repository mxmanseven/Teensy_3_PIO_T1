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
WheelManager wm;
EepromIic eepromIic;
EnduroManager em;

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
    //wmTest();
    #endif

    #if ROUTE_DEBUG == 1
    RouteTest();
    #endif
    
    #if ENDURO_MANAGER_DEBUG == 1
    EnduroManagerTest();
    #endif
    
    em.startEnduro();
}

float tenthMilesToPossiable = 0;
int16_t secondsOffPace = 0;

void loop() {
     delay(1000);

    wm.AddTickRaw();

    em.getRaceData( 
        tenthMilesToPossiable,
        secondsOffPace);

    float distance = wm.GetTotalDistance();

    Serial.printf("distance: %f\n", distance);
    Serial.printf("speed: %f\n", wm.GetSpeed(2));
    Serial.printf("tenthMilesToPossiable: %f\n", tenthMilesToPossiable);
    Serial.printf("secondsOffPace: %f\n\n", secondsOffPace);


    // eepromIic.write_byte(0, 27);
    // delay(100);

    // int readValue = 0;
    // readValue = eepromIic.read_byte(0);
    // Serial.println("eeprom " + String(readValue));

    // int unixTime = timeKnh.getUnitxTime();
    // Serial.println(String(unixTime)); 
}