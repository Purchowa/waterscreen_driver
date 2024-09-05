#ifndef DATETIME_PARSER_C_
#define DATETIME_PARSER_C_

#include "datetime_parser.h"

#include <stdio.h>
#include <string.h>

#define DATE_STRS_LEN       4 // Size includes null-terminantion
#define DATETIME_ARGS_COUNT 7

#define MONTH_COUNT   12
#define WEEKDAY_COUNT 7

#define NOT_FOUND_VALUE -1

static const char monthMap[MONTH_COUNT][DATE_STRS_LEN] = {
    { "Jan" }, { "Feb" }, { "Mar" }, { "Apr" }, { "May" }, { "Jun" },
    { "Jul" }, { "Aug" }, { "Sep" }, { "Oct" }, { "Nov" }, { "Dec" },
};

static const char weekdayMap[WEEKDAY_COUNT][DATE_STRS_LEN] = { { "Mon" }, { "Tue" }, { "Wed" }, { "Thu" },
                                                               { "Fri" }, { "Sat" }, { "Sun" } };

/**
 * @param srcStr source string to match from `collection`
 * @param collection collection has fixed size strings of length 3
 * @param collectionSize number of strings in the `collection`
 * @return index of the matching string from `collection`
 * @return `-1` if no matching string was found
 */
static int findMatchingStr( const char *srcStr, const char collection[][DATE_STRS_LEN], const size_t collectionSize )
{
    static const int equalRV = 0;
    for ( size_t i = 0; i < collectionSize; ++i )
    {
        char strToFind[DATE_STRS_LEN] = { 0 };
        memcpy( strToFind, collection[i], DATE_STRS_LEN );
        if ( strcmp( strToFind, srcStr ) == equalRV )
            return i;
    }

    return NOT_FOUND_VALUE;
}

bool getDatetimeFromDateHeader( const char *datetimeStr, Datetime_t *datetime )
{
    char weekdayStr[DATE_STRS_LEN] = { 0 };
    char monthStr[DATE_STRS_LEN]   = { 0 };

    uint8_t   day;
    const int assignedArgs =
        sscanf( datetimeStr, "%3s, %2u %3s %4u %2u:%2u:%2u", weekdayStr, &day, monthStr, &datetime->date.year,
                &datetime->time.hour, &datetime->time.minute, &datetime->time.second );
    datetime->date.day = day; // scanf is broken...

    if ( assignedArgs != DATETIME_ARGS_COUNT )
        return false;

    const int monthIndex = findMatchingStr( monthStr, monthMap, MONTH_COUNT );
    if ( monthIndex == NOT_FOUND_VALUE )
        return false;
    datetime->date.month = monthIndex + 1;

    const int weekdayIndex = findMatchingStr( weekdayStr, weekdayMap, WEEKDAY_COUNT );
    if ( weekdayIndex == NOT_FOUND_VALUE )
        return false;
    datetime->date.weekday = (Weekday_t)weekdayIndex;

    return true;
}

#define JANUARY  1
#define FEBRUARY 2
#define MARCH    3
#define NOVEMBER 11

#define GMT_1 1
#define GMT_2 2

bool getLocalDatetimeFromDateHeader( const char *datetimeStr, Datetime_t *datetime )
{
    const bool wasParsed = getDatetimeFromDateHeader( datetimeStr, datetime );
    if ( !wasParsed )
        return false;

    if ( MARCH < datetime->date.month && datetime->date.month < NOVEMBER )
        datetime->time.hour += GMT_2;
    else
        datetime->time.hour += GMT_1;

    return true;
}


/**
 * Mathematical formula for getting day of the week based on date in Gregorian clendar - `Zeller's congruence`
 */
Weekday_t getDayOfTheWeek( const uint16_t year, const uint8_t month, const uint8_t day )
{
    uint16_t y = year;
    uint8_t  m = month;
    if ( month == JANUARY || month == FEBRUARY )
    {
        m += MONTH_COUNT;
        --y;
    }
    const uint16_t k = y % 100;
    const uint16_t j = y / 100;

    const uint8_t h = day + 13 * ( m + 1 ) / 5 + k + k / 4 + j / 4 + 5 * j;

    const Weekday_t dayOfTheWeek = ( h + 5 ) % 7;
    return dayOfTheWeek;
}

#endif /* DATETIME_PARSER_C_ */
