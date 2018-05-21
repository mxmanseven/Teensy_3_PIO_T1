#ifndef ENDURO_MANAGER_H_
#define ENDURO_MANAGER_H_

#include "Arduino.h"
#include "RouteEntry.h"
#include "WheelManager.h"

#ifndef ENDURO_MANAGER_DEBUG
#define ENDURO_MANAGER_DEBUG 1
#endif

#if ENDURO_MANAGER_DEBUG == 1
int EnduroManagerTest();
#endif

class EnduroManager
{
    public:
        EnduroManager();
        
        WheelManager wm;

        // how do we get possiables form routes?
        // we only care about the next one possiable.

        // keep track of the last route entry.
        // this may be a reset, free, speed, or know
        // This gives us a know point of time and distance.
        // fromm this and currentRouteSpeed we can calculate
        // where the next possiable is and what time we should be there
        uint8_t lastRouteEntryIndex;

        // store off the last one so we don't have 
        // to look it up all the time
        RouteEntry lastRouteEntry;

        uint8_t nextRouteEntryIndex;
        RouteEntry nextRouteEntry;

        // getNextRouteEntry updates lastRouteEntry 
        // and NextRouteEntry
        int8_t getNextRouteEntry(
            uint8_t &freeMinutes);

        int8_t getRaceData(
            float &tenthMilesToPossiable,
            int16_t &secondsOffPace);

        // gets new route entries 
        // if we have gone through the last route
        int updateRoutes(
            float totalDistanceTenthMile);

        int getTenthMilesToNextPossiable(
            float totalDistanceTenthMile,
            float &tenthMilesToPossiable,
            uint8_t &nextPossiableCountAtSpeed,
            int &minutesPerPossiable);

        // the last route may not be a speed,
        // keep current route speed here
        uint8_t currentRouteSpeed;

        uint16_t currentSpeedStartTenthMile;

        // for the last speed, the expected time to be there.
        int currentSpeedStartOnTimeSeconds;

        // need to know if when we get to the next route entry
        // all route entries are distance based
        // if current distance >= distance of next

        int8_t startEnduro();

        bool canStartEnduro();

        int secondsFromTenthMilesAndMph(
            uint16_t tenthMiles,
            uint8_t speedMph);

            
        int secondsFromTenthMilesAndMph(
            float tenthMiles,
            uint8_t speedMph);

};
#endif