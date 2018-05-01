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

int TimeServicKnh::getUnitxTime()
{
    DateTime now = RTC.now();
    return now.unixtime();
}