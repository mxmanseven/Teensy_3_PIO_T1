#include "Route.h"
#include "SpeedInterval.h"
#include "EepromIic.h"

#if ROUTE_DEBUG == 1
void RouteTest()
{

  Route::setRouteCount(0);

  // knh todo - see why route type is not getting set or read correctly.
  int valid = Route::addEntry(
    10, // start mile
    20, // end mile
    6, // speed
    7, // free minutes
    RouteType::SpeedChange);

  if (valid != 0)
  {
    Serial.println("RouteTest addEntry() failed "
      + String(valid));
  }

  uint16_t startTenthMile = 0;
  uint16_t endTenthMile = 0;
  uint8_t speed = 0;
  uint8_t freeMinutes = 0;
  RouteType routeType = RouteType::FreeTime;

  Route::getEntry(
    0,
    startTenthMile,
    endTenthMile,
    speed,
    freeMinutes,
    routeType);

  String r =Route::ToStringConsole(
    startTenthMile,
    endTenthMile,
    speed,
    freeMinutes,
    routeType);

  Serial.println("RouteTest:" + r);

}
#endif

Route::Route()
{
}

uint8_t Route::getRouteCount()
{
  //knh todo  if it is bogus, use zero.
  uint8_t r = EepromIic::read_byte(ROUTE_COUNT_ADDRESS);
  Serial.println("Route::getRouteCount raw count: " + String(r));
  if (r > MAX_ROUTES) r = 0;
  return r;
}

void Route::setRouteCount(uint8_t count)
{
  Serial.println("Route::setRouteCount: " + String(count));
  EepromIic::write_byte(ROUTE_COUNT_ADDRESS, count);
}

int8_t Route::addEntry(
  uint16_t startTenthMile,
  uint16_t endTenthMile,
  uint8_t speed,
  uint8_t freeMinutes,
  RouteType routeType)
{
  int routeCount = getRouteCount(); 

  #if ROUTE_DEBUG == 1
    Serial.println("Route addEntry routeCount: " + String(routeCount));
  #endif

  if ((routeCount + 1) >= MAX_ROUTES) return -1;

  uint8_t validRoute = validateNewRoute(
    startTenthMile,
    endTenthMile,
    speed,
    freeMinutes,
    routeType);

  if(validRoute < 0) 
  {
    #if ROUTE_DEBUG == 1
      Serial.println("Route::getEntry() validation failed! validRoute: " + String(validRoute));
    #endif
    return -1;
  }

  // write this route to the next spot in eeprom
  int addrToWrite = STARTTING_ADDRESS + (routeCount * BYTES_PER_ROUTE);

  #if ROUTE_DEBUG == 1
    Serial.println("Route::getEntry() addrToWrite: " + String(addrToWrite));
  #endif

  EepromIic::write_byte(addrToWrite++, startTenthMile >> 8); // MSB
  EepromIic::write_byte(addrToWrite++, startTenthMile & 0xFF); // LSB
  EepromIic::write_byte(addrToWrite++, endTenthMile >> 8); // MSB
  EepromIic::write_byte(addrToWrite++, endTenthMile & 0xFF); // LSB
  EepromIic::write_byte(addrToWrite++, speed);
  EepromIic::write_byte(addrToWrite++, freeMinutes);
  EepromIic::write_byte(addrToWrite++, (uint8_t)routeType);

  routeCount++;
  setRouteCount(routeCount);
  
  return 0;  
}

int8_t Route::getEntry(
      uint8_t entryIndex,
      uint16_t& startTenthMile,
      uint16_t& endTenthMile,
      uint8_t& speed,
      uint8_t& freeMinutes,
      RouteType& routeType)
{
  uint8_t routeCount = getRouteCount();

#if ROUTE_DEBUG == 1
  Serial.println("Route::getEntry() entryIndex: " + String(entryIndex));
  Serial.println("Route::getEntry() routeCount: " + String(routeCount));
#endif

  if (entryIndex >= routeCount) return -1;

  int addrToRead = STARTTING_ADDRESS + (routeCount * BYTES_PER_ROUTE);

  Serial.println("Route::getEntry() addrToRead: " + addrToRead);

#if ROUTE_DEBUG == 1
  Serial.println("Route::getEntry() addrToRead: " + addrToRead);
#endif

  startTenthMile = EepromIic::read_byte(addrToRead++) << 8; // MSB
  startTenthMile |= EepromIic::read_byte(addrToRead++); // LSB
  endTenthMile = EepromIic::read_byte(addrToRead++) << 8; // MSB
  endTenthMile |= EepromIic::read_byte(addrToRead++); // LSB

  speed = EepromIic::read_byte(addrToRead++);
  freeMinutes = EepromIic::read_byte(addrToRead++);
  routeType = (RouteType)EepromIic::read_byte(addrToRead++);

  return 0;
}

uint8_t Route::validateNewRoute(
  uint16_t startTenthMile,
  uint16_t endTenthMile,
  uint8_t speed,
  uint8_t freeMinutes,
  RouteType routeType)
  {
    switch(routeType){
      case RouteType::SpeedChange:
      {
        // check that speed is valid
        float miles = 0;
        int minutes = 0;
        int validSpeed = SpeedInterval::GetMilesAndMinutesBySpeed(
          speed,
          miles,
          minutes);
        return validSpeed;
      }
      case RouteType::MiliageRest:
      {
        return startTenthMile <= endTenthMile ? 1 : 0;
      }
      case RouteType::KnownControl:
      {
        return 1;
      }
      case RouteType::EndBy:
      {
        return 1;
      }
      case RouteType::FreeTime:
      {
        return 1;
      }
    }
    return 0;
  }

String Route::ToStringConsole(
  uint16_t startTenthMile,
  uint16_t endTenthMile,
  uint8_t speed,
  uint8_t freeMinutes,
  RouteType routeType)
{
  return "StartTenthMile: " + String(startTenthMile)
    + " endTenthMile: " + String(endTenthMile)
    + " speed: " + String(speed)
    + " routeType: " + String((int)routeType)
    + " freeMinutes: " + String(freeMinutes);
}

