#ifndef SPEED_INTERVAL_H_
#define SPEED_INTERVAL_H_

#include <avr/io.h>

class SpeedInterval 
{
  public:
  static int GetMilesAndMinutesBySpeed(
    int speed, 
    float& miles, 
    int& minutes);
  
  // build static const table
  // Mile and Minute by speed
  // speeds 1 - 100 ish
  static uint8_t MileTehthssAndMinutesBySpeed[][2];
};

#endif

