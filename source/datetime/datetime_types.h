#ifndef DATETIME_TYPES_H_
#define DATETIME_TYPES_H_

#include <stdint.h>

typedef enum
{
    Monday,
    Tuesday,
    Wednesday,
    Thursday,
    Friday,
    Saturday,
    Sunday,
    WEEKDAY_COUNT
} Weekday_t;

typedef enum
{
    January = 1,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December,
    MONTH_COUNT = December
} Month_t;

typedef struct
{
    uint16_t  year;  // 1970 - 2099
    Month_t   month; // 1 - 12
    uint8_t   day;   // 1 - 31
    Weekday_t weekday;
} Date_t;

typedef struct
{
    uint8_t hour;   // 0 - 23
    uint8_t minute; // 0 - 59
    uint8_t second; // 0 - 59
} Time_t;

typedef struct
{
    Date_t date;
    Time_t time;
} Datetime_t;

typedef struct
{
    uint8_t day;
    Month_t month;
} ShortDate_t;

#endif /* DATETIME_TYPES_H_ */
