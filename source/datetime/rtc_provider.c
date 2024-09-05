#include "rtc_provider.h"

#include "datetime/datetime_parser.h"

#include <fsl_rtc.h>


void initRTC()
{
    RTC_Init( RTC );
    RTC_EnableTimer( RTC, true );
}

void setRTCDatetime( const Datetime_t *datetime )
{
    RTC_EnableTimer( RTC, false );

    const rtc_datetime_t rtcDatetime = { .year   = datetime->date.year,
                                         .month  = datetime->date.month,
                                         .day    = datetime->date.day,
                                         .hour   = datetime->time.hour,
                                         .minute = datetime->time.minute,
                                         .second = datetime->time.second };

    RTC_SetDatetime( RTC, &rtcDatetime );
    RTC_EnableTimer( RTC, true );
}
Datetime_t getRTCDatetime()
{
    rtc_datetime_t rtcDatetime;
    RTC_GetDatetime( RTC, &rtcDatetime );

    Datetime_t datetime;
    datetime.date.year  = rtcDatetime.year;
    datetime.date.month = rtcDatetime.month;
    datetime.date.day   = rtcDatetime.day;

    datetime.date.weekday = getDayOfTheWeek( datetime.date.year, datetime.date.month, datetime.date.day );

    datetime.time.hour   = rtcDatetime.hour;
    datetime.time.minute = rtcDatetime.minute;
    datetime.time.second = rtcDatetime.second;

    return datetime;
}
