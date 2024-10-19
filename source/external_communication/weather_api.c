#include "weather_api.h"

#include "datetime/datetime_parser.h"
#include "weather_parser.h"

#include "config/wifi_cfg.h"
#include "external/wlan/wlan_mwm.h"

#include <string.h>

#define GET_BUFFER_LEN      2046
#define CONTENT_LEN_STR_LEN 32

#define CONVERSION_FAILED_VALUE 0

static char s_getBuffer[GET_BUFFER_LEN];

WeatherApiStatusCode_t getWeather( Weather_t *weather )
{
    memset( s_getBuffer, 0, GET_BUFFER_LEN );

    http_GET( WEATHER_API_URL, s_getBuffer );

    size_t requestLen = strlen( s_getBuffer );
    if ( requestLen == CONVERSION_FAILED_VALUE )
        return HttpGetError;

    char contentLenStr[CONTENT_LEN_STR_LEN] = { 0 };
    http_head_parser( s_getBuffer, contentLenStr, "Content-Length:" );

    size_t contentLen = atol( contentLenStr );
    if ( contentLen == CONVERSION_FAILED_VALUE )
        return HttpGetError;

    const char *weatherContent = s_getBuffer + ( requestLen - contentLen );

    if ( !fromJsonIntoWeather( weatherContent, weather ) )
        return WeatherParsingError;

    return Success;
}

WeatherApiStatusCode_t getDatetime( Datetime_t *datetime )
{
    memset( s_getBuffer, 0, GET_BUFFER_LEN );

    http_GET( WEATHER_API_URL, s_getBuffer );

    size_t requestLen = strlen( s_getBuffer );
    if ( requestLen == CONVERSION_FAILED_VALUE )
        return HttpGetError;

    char dateStr[CONTENT_LEN_STR_LEN] = { 0 };
    http_head_parser( s_getBuffer, dateStr, "Date: " );

    if ( !getLocalDatetimeFromDateHeader( dateStr, datetime ) )
        return DatetimeParsingError;

    return Success;
}
