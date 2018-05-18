#ifndef ENDURO_MANAGER_H_
#define ENDURO_MANAGER_H_

#include "Arduino.h"
#include "RouteEntry.h"

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

        int8_t getNextRouteEntry(
            uint8_t &freeMinutes);

        int8_t getRaceData(
            uint16_t &tenthMilesToPossiable,
            int16_t &secondsOffPace
        );

        // the last route may not be a speed,
        // keep current route speed here
        uint8_t currentRouteSpeed;

        uint16_t currentSpeedStartTenthMile;

        // need to know if when we get to the next route entry
        // all route entries are distance based
        // if current distance >= distance of next

        int8_t startEnduro();

        bool canStartEnduro();

};
#endif