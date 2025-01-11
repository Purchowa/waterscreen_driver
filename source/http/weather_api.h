#ifndef WEATHER_API_H_
#define WEATHER_API_H_

#include "weather_types.h"
#include "http_return_codes.h"


const Weather_t  *getWeather();
HttpReturnCodes_t httpGetWeather();

#endif /* WEATHER_API_H_ */
