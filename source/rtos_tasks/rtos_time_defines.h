#ifndef RTOS_TIME_DEFINES_H_
#define RTOS_TIME_DEFINES_H_

#include <stdint.h>

#define SECOND_MS 1000
#define MINUTE_MS ( SECOND_MS * 60 )
#define HOUR_MS   ( SECOND_MS * 3600 )


#define PRESENTING_DELAY_MS 16

#define BETWEEN_PICTURES_DELAY_MS 500

#define WIFI_TASK_DELAY_MS SECOND_MS

#define DELAY_TO_WIFI_TASK_CALLS( delayMS ) ( delayMS / WIFI_TASK_DELAY_MS )

#define WATERSCREEN_CONFIG_GET_CALLS_NUMBER DELAY_TO_WIFI_TASK_CALLS( ( 5 * SECOND_MS ) )
#define WEATHER_GET_CALLS_NUMBER            DELAY_TO_WIFI_TASK_CALLS( HOUR_MS )

#if WATERSCREEN_CONFIG_GET_CALLS_NUMBER < DELAY_TO_WIFI_TASK_CALLS( WIFI_TASK_DELAY_MS ) ||                            \
    WEATHER_GET_CALLS_NUMBER < DELAY_TO_WIFI_TASK_CALLS( WIFI_TASK_DELAY_MS )
#error "The delay of a WiFi task cannot be greater than the other delays contained in it"
#endif

typedef uint32_t delayMs_t;


#endif /* RTOS_TIME_DEFINES_H_ */
