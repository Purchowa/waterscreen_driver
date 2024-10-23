#ifndef RTOS_TIME_DEFINES_H_
#define RTOS_TIME_DEFINES_H_

#include <stdint.h>

#define SECOND_MS 1000
#define MINUTE_MS ( SECOND_MS * 60 )
#define HOUR_MS   ( SECOND_MS * 3600 )

#define PRESENTING_DELAY_MS 16

#define BETWEEN_PICTURES_DELAY_MS 500

#define WATERSCREEN_CONFIG_GET_INTERVAL ( 5 * SECOND_MS )
#define WEATHER_GET_INTERVAL            HOUR_MS


typedef uint32_t delayMs_t;


#endif /* RTOS_TIME_DEFINES_H_ */
