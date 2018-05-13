#include "Route.h"
#include "SpeedInterval.h"
#include "EepromIic.h"

#if ROUTE_DEBUG == 1
void RouteTest()
{
  // for (int j = 0; j < 512; j++)
  // {
  //   EepromIic::write_byte(j, 0);
  // }

  Route::setRouteCount(0);
  int index = 0;
  for (int i = 0; i < 50; i +=5)
  {
    Serial.println("i: " + String(i));
    int valid = Route::addEntry(
      i, // start mile
      i+5, // end mile
      i+6, // speed
      i+7, // free minutes
      (RouteType)((int)(i*2.2)%5));

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

    Serial.println("RouteTest getEntry()");
    int nResult = Route::getEntry(
      index++,
      startTenthMile,
      endTenthMile,
      speed,
      freeMinutes,
      routeType);

    Serial.println("RouteTest: getEntry result: '" + String(nResult) + "'");
    Serial.flush();
  }  
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
    //Serial.println("Route addEntry routeCount: '" + String(routeCount) + "'");
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
      Serial.println("Route::addEntry() validation failed! validRoute: " + String(validRoute));
    #endif
    return -1;
  }

  // write this route to the next spot in eeprom
  int addrToWrite = STARTTING_ADDRESS + (routeCount * BYTES_PER_ROUTE);

  #if ROUTE_DEBUG == 1
    Serial.println("Route::addEntry() addrToWrite: " + String(addrToWrite));
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
  Serial.println("getEntry top");

  uint8_t routeCount = getRouteCount();

#if ROUTE_DEBUG == 1
  Serial.println("Route::getEntry() entryIndex: " + String(entryIndex));
  Serial.println("Route::getEntry() routeCount: " + String(routeCount));
#endif

  if (entryIndex >= routeCount) return -1;

  //knh todo - use addrToRead should be based off of entryIndex
  int addrToRead = STARTTING_ADDRESS + ((entryIndex) * BYTES_PER_ROUTE);

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

   #if ROUTE_DEBUG == 1
  String r =Route::ToStringConsole(
    startTenthMile,
    endTenthMile,
    speed,
    freeMinutes,
    routeType);
  Serial.println("RouteTest: " + r);
  #endif

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

