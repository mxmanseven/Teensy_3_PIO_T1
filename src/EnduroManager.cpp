#include "EnduroManager.h"
#include "Route.h"
//#include "Android.h"
#include "WheelManager.h"
#include "SpeedInterval.h"

int8_t EnduroManager::startEnduro()
{
    lastRouteEntryIndex = 0;
    nextRouteEntryIndex = 0;
    currentRouteSpeed = 0;
    currentSpeedStartTenthMile = 0;
    lastRouteEntry.speed = 0;
    lastRouteEntry.startTenthMile = 0;
    lastRouteEntry.endTenthMile = 0;
    lastRouteEntry.routeType = RouteType::SpeedChange;    
    nextRouteEntry.speed = 0;
    nextRouteEntry.startTenthMile = 0;
    nextRouteEntry.endTenthMile = 0;
    nextRouteEntry.routeType = RouteType::SpeedChange;
    
    uint8_t routeCount = Route::getRouteCount();

    if (routeCount == 0) 
    {
        Serial.printf("EnduroManager::startEnduro cannot start");
        return -1;
    }

    lastRouteEntryIndex = 0;
    uint8_t freeMinutes = 0;

    Route::getEntry(
        lastRouteEntryIndex, 
        lastRouteEntry.startTenthMile,
        lastRouteEntry.endTenthMile,
        lastRouteEntry.speed,
        freeMinutes,
        lastRouteEntry.routeType);

    // the first route should always be a speed
    if (lastRouteEntry.routeType == RouteType::SpeedChange)
    {
        currentRouteSpeed = lastRouteEntry.speed;
        currentSpeedStartTenthMile = lastRouteEntry.startTenthMile;
    }
    else
    {
        Serial.printf("EnduroManager::StartEnduro ERROR, first route is not a speed");
        return -2;
    }

    if(routeCount > 1)
    {
        // get next route
        nextRouteEntryIndex = lastRouteEntryIndex + 1;
        uint8_t freeMinutesTemp = 0;
        Route::getEntry(
            nextRouteEntryIndex, 
            nextRouteEntry.startTenthMile,
            nextRouteEntry.endTenthMile,
            nextRouteEntry.speed,
            freeMinutesTemp,
            nextRouteEntry.routeType);
    }
}

int8_t EnduroManager::getRaceData(
            uint16_t &tenthMilesToPossiable,
            int16_t &secondsOffPace)
{
    float totalDistanceTenthMile = WheelManager::GetTotalDistance() * 10;

    if(totalDistanceTenthMile >= nextRouteEntry.startTenthMile)
    {
        // get next route entry

    }

    // get miles per possiable
    float milesPerPossiable = 0;
    int minutesPerPossiable = 0;

    SpeedInterval::GetMilesAndMinutesBySpeed(
        currentRouteSpeed,
        milesPerPossiable,
        minutesPerPossiable);
    
    // float distanceIntoSpeedRoute = totalDistance - (currentSpeedStartTenthMile / 10.0);

    uint8_t possiablCount = currentSpeedStartTenthMile;

    return 0;
}

int8_t EnduroManager::getNextRouteEntry(uint8_t &freeMinutes)
{
    lastRouteEntryIndex++;
    int nResult = 0;

    nResult = Route::getEntry(
        lastRouteEntryIndex, 
        lastRouteEntry.startTenthMile,
        lastRouteEntry.endTenthMile,
        lastRouteEntry.speed,
        freeMinutes,
        lastRouteEntry.routeType);

    Serial.printf("EnduroManager::getNextRouteEntry getentry failed: %d", 
        nResult);

    return -1;

    uint8_t routeCount = Route::getRouteCount();

    if(routeCount > lastRouteEntryIndex)
    {
        // get next route
        nextRouteEntryIndex = lastRouteEntryIndex + 1;
        uint8_t freeMinutesTemp = 0;
        Route::getEntry(
            nextRouteEntryIndex, 
            nextRouteEntry.startTenthMile,
            nextRouteEntry.endTenthMile,
            nextRouteEntry.speed,
            freeMinutesTemp,
            nextRouteEntry.routeType);
    }
}

bool EnduroManager::canStartEnduro()
{
    uint8_t routeCount = Route::getRouteCount();
    return routeCount > 0;
}