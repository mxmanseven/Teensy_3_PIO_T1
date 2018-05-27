/*
This is an American Motorcycle Association Enduro computer.
Given a route of speed averages, resets, free time, and known controls
this computer will give the user their pace, distance, and time,
all the information needed to win!

   Hardware setup:
Microcontroller:
   Teensy 3.0 with NXP MK20d
RTC / EEPROM:
   ZS-042 break out board
   DS3231  RTC
   AT24C32 32kbit eeprom

Connections:
   ZS-042 is connected to the teensy with i2c.
   IIC sac and sad lines are on pins 16 and 17
   it is powered with 3.3v from the teensy
   External pull up resistors are used.
   I think they are 5k.
  
*/





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
    

    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(PIN_A7, INPUT_PULLUP); // Pushbutton
    //em.startEnduro();
}

float tenthMilesToPossiable = 0;
int16_t secondsOffPace = 0;

uint32_t i = 0;


bool ledOn = false;
void loop() {
    //if(i++ == 0) em.startEnduro();



    uint8_t read = digitalRead(PIN_A7);
    Serial.printf("read %d\n", read);
    
    if(!read) ledOn = !ledOn;

    if(ledOn)
        digitalWrite(LED_BUILTIN, HIGH);
    else
        digitalWrite(LED_BUILTIN, LOW);

     delay(1000);

    //wm.AddTickRaw();

    // em.getRaceData( 
    //     tenthMilesToPossiable,
    //     secondsOffPace);

    // float distance = wm.GetTotalDistance();

    // Serial.printf("getRaceSeconds: %d\n", timeKnh.getRaceSeconds());
    // Serial.printf("distance: %f\n", distance);
    // Serial.printf("speed: %f\n", wm.GetSpeed(2));
    // Serial.printf("tenthMilesToPossiable: %f\n", tenthMilesToPossiable);
    // Serial.printf("secondsOffPace: %d\n\n", secondsOffPace);


    // eepromIic.write_byte(0, 27);
    // delay(100);

    // int readValue = 0;
    // readValue = eepromIic.read_byte(0);
    // Serial.println("eeprom " + String(readValue));

    // int unixTime = timeKnh.getUnitxTime();
    // Serial.println(String(unixTime)); 
}