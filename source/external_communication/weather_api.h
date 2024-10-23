#ifndef WEATHER_API_H_
#define WEATHER_API_H_

#include "datetime/datetime_types.h"
#include "weather_types.h"
#include "http_return_codes.h"


HttpReturnCodes_t httpGetDatetime( Datetime_t *datetime );
HttpReturnCodes_t httpGetWeather( Weather_t *weather );

#endif /* WEATHER_API_H_ */
