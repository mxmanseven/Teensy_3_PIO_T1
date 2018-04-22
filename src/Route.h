#include <avr/io.h>
#include "RouteEntry.h"

#ifndef ROUTE_H_
#define ROUTE_H_

class Route
{
  public:
    Route();
    
    // these need to be in persistant storage, how do we do that?
    // write to flash
    // Need to store current and max index.
    static const uint8_t MAX_ROUTES = 50;

    // usedRouteEntryCount, incrimented for each route entry added
    uint8_t usedRouteEntryCount = 0;
    RouteEntry* routeEntries[MAX_ROUTES];

    // addSpeedEntry() adds route routeEntries[usedRouteEntryCount], then incirments count.
    int8_t addSpeedEntry(uint16_t startTenthMile, uint8_t speed);  
};

#endif

