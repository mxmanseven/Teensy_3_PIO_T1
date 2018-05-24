#ifndef TIME_SERVICE_H_
#define TIME_SERVICE_H_


#include <DS3231.h>


class TimeServicKnh
{
    public:
        DS3231 Clock;

        RTClib RTC;

        byte Year = 20;
        byte Month = 04;
        byte Date = 29;
        byte DoW = 1;
        byte Hour = 9;
        byte Minute = 1;
        byte Second = 0;

        int setTime();


        // on race start, get time to base offset
        // use internal mills() for speed calc.
        uint32_t getUnitxTime();
};

#endif

