#include "EnduroManager.h"
#include "Route.h"
//#include "Android.h"
#include "WheelManager.h"
#include "SpeedInterval.h"

#if ENDURO_MANAGER_DEBUG == 1
int EnduroManagerTest()
{
    int nResult = 0;

    Route route;

    EnduroManager em;

    route.ClearAllRoutes();
    // 1
    route.addEntry(
        0, // start tenthmile
        40, // end tenth mile
        6, // speed mph
        0,  // free minutes
        RouteType::SpeedChange);
    // 2
    route.addEntry(
        40, // start tenthmile
        45, // end tenth mile
        12, // speed mph
        0,  // free minutes
        RouteType::SpeedChange);
    // 3
    route.addEntry(
        45, // start tenthmile
        46, // end tenth mile
        6, // speed mph
        0,  // free minutes
        RouteType::SpeedChange);
    // 4
    route.addEntry(
        46, // start tenthmile
        60, // end tenth mile
        7, // speed mph
        0,  // free minutes
        RouteType::SpeedChange);

    // uint8_t freeMinutes = 0;
    // RouteEntry r(0, 0, 0, RouteType::SpeedChange);
    // for (int i = 0; i < 4; i++)
    // {
    //     int nResult = Route::getEntry(
    //     i++,
    //     r.startTenthMile,
    //     r.endTenthMile,
    //     r.speed,
    //     freeMinutes,
    //     r.routeType);

    //     if(nResult)
    //     {
    //         Serial.printf("WM::test getEntry failed: %d", nResult);
    //     }

    //     String routeString =Route::ToStringConsole(
    //         r.startTenthMile,
    //         r.endTenthMile,
    //         r.speed,
    //     freeMinutes,
    //     r.routeType);
    //     Serial.println("RouteTest obj: " + routeString);
    // }
    // nResult = em.startEnduro();
    
    return nResult;
}
#endif

EnduroManager::EnduroManager()
{

}

int8_t EnduroManager::startEnduro()
{
    // zero everything out
    // get first route which must be a speed

    // knh todo - zero clock

    wm.SetDistanceToZero();  
    lastRouteEntryIndex = 0;
    nextRouteEntryIndex = 0;
    currentRouteSpeed = 0;
    currentSpeedStartTenthMile = 0;
    currentSpeedStartOnTimeSeconds = 0;
    lastRouteEntry.speed = 0;
    lastRouteEntry.startTenthMile = 0;
    lastRouteEntry.endTenthMile = 0;
    lastRouteEntry.routeType = RouteType::SpeedChange;    
    nextRouteEntry.speed = 0;
    nextRouteEntry.startTenthMile = 0;
    nextRouteEntry.endTenthMile = 0;
    nextRouteEntry.routeType = RouteType::SpeedChange;
    enduroStartTimeSeconds =  timeService.getUnitxTime();

    uint8_t freeMinutes = 0;
    
    uint8_t routeCount = Route::getRouteCount();

    if (routeCount == 0) 
    {
        Serial.printf("EnduroManager::startEnduro cannot start, route count: %d\n",
                      routeCount);
        return -1;
    }

    int8_t nResult = getNextRouteEntry(freeMinutes);

    if (nResult) 
    {
        Serial.printf("EnduroManager::startEnduro getNextRouteEntry FAILED %d\n", 
                      nResult);
        return -2;
    }
    
    // the first route should always be a speed
    if (lastRouteEntry.routeType == RouteType::SpeedChange)
    {
        currentRouteSpeed = lastRouteEntry.speed;
        currentSpeedStartTenthMile = lastRouteEntry.startTenthMile;
    }
    else
    {
        Serial.printf("EnduroManager::StartEnduro ERROR, first route is not a speed");
        return -3;
    }

    return 0;
}

int EnduroManager::updateRoutes(
    float totalDistanceTenthMile)
{
    int nResult = 0;    
    
    uint8_t freeMinutes = 0;
    float milesChange = 0.0;
    
    Top:

    if(totalDistanceTenthMile >= nextRouteEntry.startTenthMile)
    {
        Serial.printf("EM::updateRoutes() gettingNextRouteEntry\n");

        RouteEntry previousRouteEntry;
        previousRouteEntry.startTenthMile = lastRouteEntry.startTenthMile;
        previousRouteEntry.endTenthMile = lastRouteEntry.endTenthMile;
        previousRouteEntry.routeType = lastRouteEntry.routeType;
        previousRouteEntry.speed = lastRouteEntry.speed;

        // get next route entry and process it
        // repeat getting next until a speed change is found 
        nResult = getNextRouteEntry(freeMinutes);

        if(nResult) 
        {
            Serial.printf("EnduroManager::updateRoutes getNextRouteEntry failed %d", 
            nResult);
            return nResult;
        }

        // handle each route type
        // all types byt speed changes will go back to Top.
        switch(lastRouteEntry.routeType)
        {
            case RouteType::SpeedChange:
            {
                // total distance of the last route speed
                uint16_t lastSpeedTotalTenthMiles = 
                    lastRouteEntry.startTenthMile - currentSpeedStartTenthMile;

                // knh todo - i think i need to add free minutes here and manager free minutes
                currentSpeedStartOnTimeSeconds +=
                    secondsFromTenthMilesAndMph((float)lastSpeedTotalTenthMiles, currentRouteSpeed)
                    + freeMinutes * 60;

                freeMinutes = 0;

                Serial.printf("EM::updateRoutes() currentSpeedStartOnTimeSeconds: %d\n",
                    currentSpeedStartOnTimeSeconds);

                currentRouteSpeed = lastRouteEntry.speed;
                currentSpeedStartTenthMile = lastRouteEntry.startTenthMile;
                
                break;
            }
            case RouteType::MiliageRest:
            { 
                milesChange = (lastRouteEntry.endTenthMile - lastRouteEntry.startTenthMile) / 10.0;
                wm.AddDistance(milesChange);
                // call self to start process from top
                goto Top;
            }
            case RouteType::KnownControl:
            case RouteType::EndBy:
            case RouteType::FreeTime:
            {
                // knh todo - finish handling route types
                goto Top;
                break;
            }
        }
    }

    return nResult;
}

int EnduroManager::getTenthMilesToNextPossiable(
    float totalDistanceTenthMile,
    float &tenthMilesToPossiable,
    uint8_t &nextPossiableCountAtSpeed,
    int &minutesPerPossiable)
{
    int nResult = 0;

    // get miles per possiable
    float milesPerPossiable = 0;
    minutesPerPossiable = 0;

    SpeedInterval::GetMilesAndMinutesBySpeed(
        currentRouteSpeed,
        milesPerPossiable,
        minutesPerPossiable);

    // Serial.printf("EM::getTenthMilesToNextPossiable, minutesPerPossiable: %d\n",
    //     minutesPerPossiable);
    Serial.printf("EM::getTenthMilesToNextPossiable, currentSpeedStartTenthMile: %d\n",
        currentSpeedStartTenthMile);
    // Serial.printf("EM::getTenthMilesToNextPossiable, totalDistanceTenthMile knh: %f\n",
    //     totalDistanceTenthMile);    

    float tenthMilesIntoSpeed = totalDistanceTenthMile - (float)currentSpeedStartTenthMile;
    
    // Serial.printf("EM::getTenthMilesToNextPossiable, tenthMilesIntoSpeed: %f\n",
    //     tenthMilesIntoSpeed);
    
    float milesIntoSpeedRoute = 
        tenthMilesIntoSpeed / 10.0;

    Serial.printf("EM::getTenthMilesToNextPossiable, milesIntoSpeedRoute: %f\n",
        milesIntoSpeedRoute);

    nextPossiableCountAtSpeed = ceil(milesIntoSpeedRoute / milesPerPossiable);

    Serial.printf("EnduroManager::getRaceData nextPossiableCountAtSpeed %d\n",
        nextPossiableCountAtSpeed);

    uint16_t nextPossiableTenthMiles = currentSpeedStartTenthMile
        + (nextPossiableCountAtSpeed * milesPerPossiable * 10.0);
        
    Serial.printf("EnduroManager::getRaceData nextPossiableTenthMiles %d\n",
        nextPossiableTenthMiles);

    tenthMilesToPossiable = nextPossiableTenthMiles - totalDistanceTenthMile;

    return nResult;
}

int8_t EnduroManager::getRaceData(
            float &tenthMilesToPossiable,
            int16_t &secondsOffPace)
{
    int nResult = 0;
    
    float totalDistanceTenthMile = wm.GetTotalDistance() * 10;
    
    nResult = updateRoutes(totalDistanceTenthMile);

    if(nResult)
    {
        Serial.printf("EM::getRaceData updateRoutes FAILED: %d\n", nResult);
        return nResult;
    }
    
    uint8_t nextPossiableCountAtSpeed = 0;
    int minutesPerPossiable = 0;
    nResult = getTenthMilesToNextPossiable(
        totalDistanceTenthMile,
        tenthMilesToPossiable,
        nextPossiableCountAtSpeed,
        minutesPerPossiable);

    if(nResult)
    {
        Serial.printf("EM::getRaceData getTenthMilesToNextPossiable FAILED: %d\n", 
            nResult);
        return nResult;
    }

    float milesFromSpeedStart = (totalDistanceTenthMile - (float)currentSpeedStartTenthMile) / 10.0;
    
    Serial.printf("EM::getRaceData milesFromSpeedStart: %f\n",
        milesFromSpeedStart);    

    int expectedSecondsHere = secondsFromTenthMilesAndMph(milesFromSpeedStart * 10, currentRouteSpeed);

    Serial.printf("EM::getRaceData expectedSecondsHere: %d\n",
        expectedSecondsHere);

    uint32_t secondsIntoEnduro = timeService.getUnitxTime() - enduroStartTimeSeconds;
    
    Serial.printf("EM::getRaceData secondsIntoEnduro: %d\n",
        secondsIntoEnduro);
    
    secondsOffPace = 
        secondsIntoEnduro - expectedSecondsHere;
        
// distance = rate * time
// rate = distance / time
// time = distance / rate

    return 0;
}

int8_t EnduroManager::getNextRouteEntry(uint8_t &freeMinutes)
{
    int nResult = 0;

    nResult = Route::getEntry(
        lastRouteEntryIndex,
        lastRouteEntry.startTenthMile,
        lastRouteEntry.endTenthMile,
        lastRouteEntry.speed,
        freeMinutes,
        lastRouteEntry.routeType);
    
    if(nResult)
    {
        Serial.printf("EM::getNextRouteEntry getEntry of last failed: %d, i: %d\n", 
            nResult, lastRouteEntryIndex);
        return -1;
    }
    
    String routeString =Route::ToStringConsole(
        lastRouteEntry.startTenthMile,
        lastRouteEntry.endTenthMile,
        lastRouteEntry.speed,
        freeMinutes,
        lastRouteEntry.routeType);

    Serial.printf("EM::getNextRouteEntry lstEntry i: %d\n",
        lastRouteEntryIndex);
    Serial.println("EM::getNextRouteEntry lstEntr" + routeString);

    uint8_t routeCount = Route::getRouteCount();

    if(routeCount > lastRouteEntryIndex)
    {
        // get next route
        //nextRouteEntryIndex = lastRouteEntryIndex + 1;
        uint8_t freeMinutesTemp = 0;
        nResult = Route::getEntry(
            lastRouteEntryIndex + 1, 
            nextRouteEntry.startTenthMile,
            nextRouteEntry.endTenthMile,
            nextRouteEntry.speed,
            freeMinutesTemp,
            nextRouteEntry.routeType);

        if(nResult)
        {    
            Serial.printf("EM::getNextRouteEntry getEntry next failed: %d, i: %d\n", 
                nResult, lastRouteEntryIndex + 1);

            return -2;
        }

        lastRouteEntry.endTenthMile = nextRouteEntry.startTenthMile;
    }
    
    lastRouteEntryIndex++;

    return nResult;
}

bool EnduroManager::canStartEnduro()
{
    uint8_t routeCount = Route::getRouteCount();
    return routeCount > 0;
}

int EnduroManager::secondsFromTenthMilesAndMph(
    float tenthMiles,
    uint8_t speedMph)
{
    return ((tenthMiles / 10.0) / (float)speedMph) * 60 * 60;
}