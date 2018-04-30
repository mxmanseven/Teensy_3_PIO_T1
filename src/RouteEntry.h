#include <avr/io.h>

#ifndef ROUTE_ENTRY_H_
#define ROUTE_ENTRY_H_

// enum class if type uint8_t
// check out http://en.cppreference.com/w/cpp/language/enum
enum class RouteType : uint8_t {SpeedChange, MiliageRest, KnownControl, EndBy };

class RouteEntry
{
  public:
  RouteEntry();

  RouteEntry(
    uint16_t startTenthMile,
    uint8_t speed,
    uint16_t endTenthMile,
    RouteType routeType
    );
  
  // we are dealing with tenth miles here so a value of 1 coresponds to 0.1 mile.
  uint16_t startTenthMile;  
  uint8_t speed;
  uint16_t endTenthMile;
  RouteType routeType;
};

#endif

