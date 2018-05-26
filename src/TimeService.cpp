 #include "TimeService.h"


int TimeServicKnh::setTime()
{
    Clock.setYear(Year);
    Clock.setMonth(Month);
    Clock.setDate(Date);
    Clock.setDoW(DoW);
    Clock.setHour(Hour);
    Clock.setMinute(Minute);
    Clock.setSecond(Second);
    return 0;
}

uint32_t TimeServicKnh::getUnixTime()
{
    DateTime now = RTC.now();
    return now.unixtime();
}

uint32_t TimeServicKnh::raceStartSeconds = 0;

int32_t TimeServicKnh::startRaceSeconds()
{
    raceStartSeconds = getUnixTime();
    return raceStartSeconds;
}

uint32_t TimeServicKnh::getRaceSeconds()
{
    return getUnixTime() - raceStartSeconds;
}
