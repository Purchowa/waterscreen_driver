#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "external_communication/weather_parser.h"

static bool compareWeather( const Weather_t *first, const Weather_t *second )
{
    bool areEqual = true;

    areEqual &= first->temperature == second->temperature;
    areEqual &= first->pressure == second->pressure;
    areEqual &= first->weatherCondition == second->weatherCondition;

    return areEqual;
}

void givenJson_weatherParser_parseToWeatherStruct()
{
    const char *weatherJson = "{\"weather\":[{\"id\": 801}],\"main\":{\"temp\":27.2,\"pressure\":1017} }";

    Weather_t       weather;
    const Weather_t expectedWeather = { .temperature = 27.2, .pressure = 1017, .weatherCondition = Clear };

    assert_true( fromJsonIntoWeather( weatherJson, &weather ) );
    assert_true( compareWeather( &weather, &expectedWeather ) );
}

void givenUnknownLowWeatherID_weatherParser_returnFalse()
{
    const char *weatherJson = "{\"weather\":[{\"id\": 0}],\"main\":{\"temp\":0,\"pressure\":0} }";
    Weather_t   weather;
    assert_false( fromJsonIntoWeather( weatherJson, &weather ) );
}

void givenUnknownHighWeatherID_weatherParser_returnFalse()
{
    const char *weatherJson = "{\"weather\":[{\"id\": 99999}],\"main\":{\"temp\":0,\"pressure\":0} }";
    Weather_t   weather;
    assert_false( fromJsonIntoWeather( weatherJson, &weather ) );
}

void givenEmptyJson_weatherParser_returnFalse()
{
    const char *weatherJson = "";

    Weather_t weather;
    assert_false( fromJsonIntoWeather( weatherJson, &weather ) );
}

void givenInvlaidJson_weatherParser_returnFalse()
{
    const char *weatherJson = "{\"ala\": 123}";

    Weather_t weather;
    assert_false( fromJsonIntoWeather( weatherJson, &weather ) );
}

void givenNullJson_weatherParser_returnFalse()
{
    Weather_t weather;
    assert_false( fromJsonIntoWeather( NULL, &weather ) );
}

int main()
{
    const struct CMUnitTest tests[] = { cmocka_unit_test( givenJson_weatherParser_parseToWeatherStruct ),
                                        cmocka_unit_test( givenUnknownLowWeatherID_weatherParser_returnFalse ),
                                        cmocka_unit_test( givenUnknownHighWeatherID_weatherParser_returnFalse ),
                                        cmocka_unit_test( givenEmptyJson_weatherParser_returnFalse ),
                                        cmocka_unit_test( givenInvlaidJson_weatherParser_returnFalse ),
                                        cmocka_unit_test( givenNullJson_weatherParser_returnFalse ) };

    return cmocka_run_group_tests_name( "JSON weather parser", tests, NULL, NULL );
}
