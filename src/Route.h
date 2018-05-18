
#include "RouteEntry.h"
#include "Arduino.h"

#ifndef ROUTE_H_
#define ROUTE_H_

// set ROUTE_DEBUG to 1 to enable debug mode
// set to anything else to dissable it.
#ifndef ROUTE_DEBUG
#define ROUTE_DEBUG 0
#endif

#if ROUTE_DEBUG == 1
void RouteTest();
#endif

class Route
{
  public:
    Route();

    static uint8_t getRouteCount();

    static void setRouteCount(uint8_t count);

    // addSpeedEntry() adds route routeEntries[usedRouteEntryCount], then incirments count.
    static int8_t addEntry(
      uint16_t startTenthMile,
      uint16_t endTenthMile,
      uint8_t speed,
      uint8_t freeMinutes,
      RouteType routeType);

    static int8_t getEntry(
      uint8_t entryIndex,
      uint16_t& startTenthMile,
      uint16_t& endTenthMile,
      uint8_t& speed,
      uint8_t& freeMinutes,
      RouteType& routeType);

    static String ToStringConsole(
      uint16_t startTenthMile,
      uint16_t endTenthMile,
      uint8_t speed,
      uint8_t freeMinutes,
      RouteType routeType);

    static int8_t ClearAllRoutes();

  private:


    static uint8_t validateNewRoute(
      uint16_t startTenthMile,
      uint16_t endTenthMile,
      uint8_t speed,
      uint8_t freeMinutes,
      RouteType routeType);

    static const uint8_t MAX_ROUTES = 50;

    // uint16_t startMile
    // uint16_t endMile
    // uint8_t  speed
    // uint8_t  freeMinutes
    // uint8_t  type
    // (3 * 8) + (2 * 16)
    // 24 + 32
    // 54 bits per route / 8 => 7
    static const int BYTES_PER_ROUTE = 7;

    static const int TOTAL_BYTES_TO_STORE = MAX_ROUTES * BYTES_PER_ROUTE;


};

#endif

