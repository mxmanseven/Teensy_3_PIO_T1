#include "RouteEntry.h"

RouteEntry::RouteEntry(  
    uint16_t startTenthMile_,
    uint8_t speed_,
    uint16_t endTenthMile_,
    RouteType routeType_  )
{
  startTenthMile = startTenthMile_;
  speed = speed_;
  endTenthMile = endTenthMile_;
  routeType = routeType_;    
}



