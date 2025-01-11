#ifndef WEATHER_TYPES_H_
#define WEATHER_TYPES_H_

#include <stdint.h>

typedef enum
{
    Rain,
    Snow,
    Mist,
    Clear,
    Clouds,
    WEATHER_CONDITION_SIZE
} WeatherCondition_t;

typedef struct
{
    float              temperature;
    uint16_t           pressure;
    WeatherCondition_t weatherCondition;
} Weather_t;

#endif /* WEATHER_TYPES_H_ */
