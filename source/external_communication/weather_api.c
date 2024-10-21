#include "weather_api.h"

#include "datetime/datetime_parser.h"
#include "weather_parser.h"

#include "config/wifi_cfg.h"
#include "external/wlan/wlan_mwm.h"
#include "http_get_utils.h"

#include <string.h>
#include <assert.h>

#define CONTENT_LEN_STR_LEN 32


HttpReturnCodes_t getWeather( Weather_t *weather )
{
    char      *weatherContent = NULL;
    const bool wasReceived    = httpGET_receiveContent( &weatherContent, WEATHER_API_URL );

    if ( !wasReceived )
        return Http_GETError;

    assert( weatherContent );
    if ( !fromJsonIntoWeather( weatherContent, weather ) )
        return Http_WeatherParsingError;

    return Http_Success;
}

HttpReturnCodes_t getDatetime( Datetime_t *datetime )
{
    char *httpGetBuffer = getClearedHttpGETBuffer();

    http_GET( WEATHER_API_URL, httpGetBuffer );

    size_t requestLen = strlen( httpGetBuffer );
    if ( requestLen == HTTP_HEAD_CONVERSION_FAILED_VALUE )
        return Http_GETError;

    char dateStr[CONTENT_LEN_STR_LEN] = { 0 };
    http_head_parser( httpGetBuffer, dateStr, "Date: " );

    if ( !getLocalDatetimeFromDateHeader( dateStr, datetime ) )
        return Http_DatetimeParsingError;

    return Http_Success;
}
