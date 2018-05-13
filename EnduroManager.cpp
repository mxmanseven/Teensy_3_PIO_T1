#include "EnduroManager.h"
#include "Route.h"

EnduroManager::EnduroManager()
    : lastRouteEntry(0, 0, 0, RouteType::SpeedChange),
    nextRouteEntry(0, 0 , 0, RouteType::SpeedChange)
{
    lastRouteEntryIndex = 0;
    nextRouteEntryIndex = 0;
}

bool EnduroManager::canStartEnduro()
{
    uint8_t routeCount = Route::getRouteCount();
    return routeCount > 0;
}