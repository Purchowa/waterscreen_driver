#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "datetime/datetime_parser.h"

static bool compareDatetime( const Datetime_t *first, const Datetime_t *second )
{
    bool areEqual = true;

    areEqual &= first->date.year == second->date.year;
    areEqual &= first->date.month == second->date.month;
    areEqual &= first->date.day == second->date.day;
    areEqual &= first->date.weekday == second->date.weekday;

    areEqual &= first->time.hour == second->time.hour;
    areEqual &= first->time.minute == second->time.minute;
    areEqual &= first->time.second == second->time.second;

    return areEqual;
}

void givenDateHeader_datetimeParser_returnTrue()
{
    const char *datetimeHeader = "Fri, 01 Jan 2019 23:59:00";

    Datetime_t       datetime         = {};
    const Datetime_t expectedDatetime = { .date = { .year = 2019, .month = January, .day = 1, .weekday = Friday },
                                          .time = { .hour = 23, .minute = 59, .second = 0 } };

    assert_true( getDatetimeFromDateHeader( datetimeHeader, &datetime ) );
    assert_true( compareDatetime( &expectedDatetime, &datetime ) );
}

void givenEmptyHeader_datetimeParser_returnFalse()
{
    const char *datetimeHeader = "";

    Datetime_t datetime;
    assert_false( getDatetimeFromDateHeader( datetimeHeader, &datetime ) );
}

void givenInvalidHeader_datetimeParser_returnFalse()
{
    const char *datetimeHeader = "whatever";

    Datetime_t datetime;
    assert_false( getDatetimeFromDateHeader( datetimeHeader, &datetime ) );
}

void givenNullHeader_datetimeParser_returnFalse()
{
    Datetime_t datetime;
    assert_false( getDatetimeFromDateHeader( NULL, &datetime ) );
}

void givenSummerTime_datetimeParser_returnTrueAndLocalDatetime()
{
    const char *datetimeHeader = "Fri, 23 Apr 2024 15:08:32";

    Datetime_t       datetime;
    const Datetime_t expectedDatetime = { .date = { .year = 2024, .month = April, .day = 23, .weekday = Friday },
                                          .time = { .hour = 17, .minute = 8, .second = 32 } };

    assert_true( getLocalDatetimeFromDateHeader( datetimeHeader, &datetime ) );
    assert_true( compareDatetime( &expectedDatetime, &datetime ) );
}

void givenWinterTime_datetimeParser_returnTrueAndLocalDatetime()
{
    const char *datetimeHeader = "Fri, 1 Nov 2024 15:08:32";

    Datetime_t       datetime;
    const Datetime_t expectedDatetime = { .date = { .year = 2024, .month = November, .day = 1, .weekday = Friday },
                                          .time = { .hour = 16, .minute = 8, .second = 32 } };

    assert_true( getLocalDatetimeFromDateHeader( datetimeHeader, &datetime ) );
    assert_true( compareDatetime( &expectedDatetime, &datetime ) );
}

void givenDates_getDayOfTheWeek_returnProperDayOfTheWeek()
{
    assert_true( getDayOfTheWeek( 2024, September, 1 ) == Sunday );
    assert_true( getDayOfTheWeek( 2024, January, 1 ) == Monday );
    assert_true( getDayOfTheWeek( 2024, February, 29 ) == Thursday );
}

int main()
{
    const struct CMUnitTest tests[] = { cmocka_unit_test( givenDateHeader_datetimeParser_returnTrue ),
                                        cmocka_unit_test( givenEmptyHeader_datetimeParser_returnFalse ),
                                        cmocka_unit_test( givenInvalidHeader_datetimeParser_returnFalse ),
                                        cmocka_unit_test( givenNullHeader_datetimeParser_returnFalse ),
                                        cmocka_unit_test( givenSummerTime_datetimeParser_returnTrueAndLocalDatetime ),
                                        cmocka_unit_test( givenWinterTime_datetimeParser_returnTrueAndLocalDatetime ),
                                        cmocka_unit_test( givenDates_getDayOfTheWeek_returnProperDayOfTheWeek ) };

    return cmocka_run_group_tests_name( "Date HTTP header parser", tests, NULL, NULL );
}
