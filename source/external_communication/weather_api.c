#include "weather_api.h"

#include "weather_parser.h"

#include "config/wifi_cfg.h"
#include "http_get_utils.h"

#include <string.h>
#include <assert.h>


static Weather_t s_weather = { .temperature = 20.01f, .pressure = 1001, .weatherCondition = Clear };

const Weather_t *getWeather()
{
    return &s_weather;
}

HttpReturnCodes_t httpGetWeather()
{
    char      *weatherContent = NULL;
    const bool wasReceived    = httpGET_receiveContent( &weatherContent, NULL, WEATHER_API_URL );

    if ( !wasReceived )
        return Http_GETError;

    assert( weatherContent );
    if ( !fromJsonIntoWeather( weatherContent, &s_weather ) )
        return Http_WeatherParsingError;

    return Http_Success;
}
