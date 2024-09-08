#ifndef WEATHER_API_H_
#define WEATHER_API_H_

#include "datetime/datetime_types.h"
#include "weather_types.h"

#include <stdbool.h>

// TODO: This status code will be needed for other things from api's. Separate it to other file.
typedef enum
{
    Success,
    HttpGetError,
    WeatherParsingError,
    DatetimeParsingError
} WeatherApiStatusCode_t;

WeatherApiStatusCode_t getDatetime( Datetime_t *datetime );
WeatherApiStatusCode_t getWeather( Weather_t *weather );

#endif /* WEATHER_API_H_ */
