#ifndef WEATHER_API_H_
#define WEATHER_API_H_

#include "datetime/datetime_types.h"
#include "weather_types.h"
#include "http_return_codes.h"


HttpReturnCodes_t getDatetime( Datetime_t *datetime );
HttpReturnCodes_t getWeather( Weather_t *weather );

#endif /* WEATHER_API_H_ */
