#include "weather_api.h"

#include "weather_parser.h"

#include "config/wifi_cfg.h"
#include "http_get_utils.h"

#include <string.h>
#include <assert.h>


HttpReturnCodes_t httpGetWeather( Weather_t *weather )
{
    char      *weatherContent = NULL;
    const bool wasReceived    = httpGET_receiveContent( &weatherContent, NULL, WEATHER_API_URL );

    if ( !wasReceived )
        return Http_GETError;

    assert( weatherContent );
    if ( !fromJsonIntoWeather( weatherContent, weather ) )
        return Http_WeatherParsingError;

    return Http_Success;
}
