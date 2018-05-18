#include <avr/io.h>
#include <Arduino.h>

#ifndef WHEEL_MANAGER_H_
#define WHEEL_MANAGER_H_

// set WHEEN_MANAGER_DEBUG to 1 to enable debug mode
// set to anything else to dissable it.
#ifndef WHEEL_MANAGER_DEBUG
#define WHEEL_MANAGER_DEBUG 0
#endif

#if WHEEL_MANAGER_DEBUG == 1
void wmTest();
#endif

class WheelManager
{
  public:

  //Default constructor
  WheelManager();

  int8_t beging();

   // wheelCircumfranceInches must be stored in non volital memory
   static float wheelCircumfranceInches;

   // tickMaxAgeSeconds defines the max age of a tick before booting it out of the deque.
   // When adding a tick to the deque, any ticks older than this get booted out.
   // This helps facilitate getting the average.   
   //const uint8_t tickMaxAgeSeconds = 30;
   // dont need if using a circlular array of adiquate size

  //float MIN_MS_BETWEEN_TICKS = 0.050f;
  static const uint16_t MIN_MS_BETWEEN_TICKS = 50;

  // valitTickCount, count of all valid ticks.
  // used to calculate total distance.
  static uint32_t validTickCount;

  // 6ft cirm at 68 mph => 15 hz
  // 15 hz * 30 seconds = 450
  // knh tood - change after testing
  static const uint16_t MAX_TICK_COUNT = 450;

    // last tick index = validTickCount - 1 % MAX_TICK_COUNT
  static uint32_t validTicks[MAX_TICK_COUNT];

  static float miliageAdjustment;

   // gets the average speed over the last time period of durationSeconds.
   // durationSeconds must be less or equal to tickMaxAgeSeconds.
   // returns zero by default
  float GetSpeed(uint8_t durationSeconds);

  float GetTotalDistance();

   // AddTickRaw, check if new tick is valid
   // if valid, tick is added to deque, old ticks are bumpped oute.
   // and vlaidTickCount++;
   // A tick is valid if it arrived 50ms after the last tick
   // at 6ft cirmfurance @ 15hz => 68 mph => 0.066 seconds between ticks.
  void AddTickRaw();

    // used for resets
  void AddDistance(float milesAdjustment);

  void ChangeDistanceAndCalibrate(float milesAdjustment);

  void SetDistanceToZero();

  //private:
};
#endif
