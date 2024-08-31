#include "weather_api.h"

#include "datetime/datetime_parser.h"
#include "weather_parser.h"

#include "config/WiFiCfg.h"
#include "external/wlan/wlan_mwm.h"

#include <string.h>

#define GET_BUFFER_LEN      2046
#define CONTENT_LEN_STR_LEN 32

#define CONVERSION_FAILED_VALUE 0


static bool getWeather( char srcBuffer[], const size_t srcBufferLen, Weather_t *weather )
{
    char contentLenStr[CONTENT_LEN_STR_LEN] = { 0 };
    http_head_parser( srcBuffer, contentLenStr, "Content-Length:" );

    size_t contentLen = atol( contentLenStr );
    if ( contentLen == CONVERSION_FAILED_VALUE )
        return false;

    const char *weatherContent = srcBuffer + ( srcBufferLen - contentLen );

    return fromJsonIntoWeather( weatherContent, weather );
}

static bool getDatetime( char srcBuffer[], const size_t srcBufferLen, Datetime_t *time )
{
    char dateStr[CONTENT_LEN_STR_LEN] = { 0 };
    http_head_parser( srcBuffer, dateStr, "Date: " );

    return getLocalDatetimeFromDateHeader( dateStr, time );
}

WeatherApiStatusCode_t getWeatherAndDatetime( Weather_t *weather, Datetime_t *datetime )
{
    static char getBuffer[GET_BUFFER_LEN];
    memset( getBuffer, 0, GET_BUFFER_LEN );

    http_GET( WEATHER_API_URL, getBuffer );

    size_t requestLen = strlen( getBuffer );
    if ( requestLen == CONVERSION_FAILED_VALUE )
        return HttpGetError;

    if ( !getWeather( getBuffer, requestLen, weather ) )
        return WeatherParsingError;

    if ( !getDatetime( getBuffer, requestLen, datetime ) )
        return DatetimeParsingError;

    return Success;
}
