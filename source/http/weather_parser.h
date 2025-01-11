#ifndef WEATHER_PARSER_H_
#define WEATHER_PARSER_H_

#include "weather_types.h"

#include <stdbool.h>

bool fromJsonIntoWeather( const char *weatherJson, Weather_t *weather );

#endif /* WEATHER_PARSER_H_ */
