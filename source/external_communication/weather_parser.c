#include "weather_parser.h"

#include <assert.h>

#include "cjson/cJSON.h"

#define NOT_FOUND -1

const static int16_t conditionIntervals[] = { 200, 600, 700, 800, 802, 900 };


static int16_t getWeatherConditionFromCode( const int16_t weatherCode )
{
    assert( ( sizeof( conditionIntervals ) / sizeof( *conditionIntervals ) ) - 1 == WEATHER_CONDITION_SIZE );

    for ( int16_t weatherIndex = 0; weatherIndex < WEATHER_CONDITION_SIZE; ++weatherIndex )
    {
        if ( conditionIntervals[weatherIndex] <= weatherCode && weatherCode < conditionIntervals[weatherIndex + 1] )
            return weatherIndex;
    }
    return NOT_FOUND;
}

static bool parseJsonWeather( const cJSON *weatherObj, Weather_t *weather )
{
    if ( weatherObj == NULL )
        return false;

    const cJSON *weatherArray = cJSON_GetObjectItemCaseSensitive( weatherObj, "weather" );
    if ( !cJSON_IsArray( weatherArray ) )
        return false;

    const cJSON *weatherElementObj = weatherArray->child;
    if ( !cJSON_IsObject( weatherElementObj ) )
        return false;

    const cJSON *weatherID = cJSON_GetObjectItemCaseSensitive( weatherElementObj, "id" );
    if ( !cJSON_IsNumber( weatherID ) )
        return false;

    const int16_t weatherConditionIndex = getWeatherConditionFromCode( (int16_t)weatherID->valueint );
    if ( weatherConditionIndex == NOT_FOUND )
        return false;
    weather->weatherCondition = (WeatherCondition_t)weatherConditionIndex;

    const cJSON *mainObj = cJSON_GetObjectItemCaseSensitive( weatherObj, "main" );
    if ( !cJSON_IsObject( mainObj ) )
        return false;

    const cJSON *temperature = cJSON_GetObjectItemCaseSensitive( mainObj, "temp" );
    if ( !cJSON_IsNumber( temperature ) )
        return false;
    weather->temperature = (float)temperature->valuedouble;

    const cJSON *pressure = cJSON_GetObjectItemCaseSensitive( mainObj, "pressure" );
    if ( !cJSON_IsNumber( pressure ) )
        return false;
    weather->pressure = pressure->valueint;

    return true;
}

bool fromJsonIntoWeather( const char *weatherJson, Weather_t *weather )
{
    cJSON     *weatherJsonObj        = cJSON_Parse( weatherJson );
    const bool wasSuccessfullyParsed = parseJsonWeather( weatherJsonObj, weather );
    cJSON_Delete( weatherJsonObj );

    return wasSuccessfullyParsed;
}
