#ifndef RTOS_TIME_DEFINES_H_
#define RTOS_TIME_DEFINES_H_

#include <stdint.h>

#define SECOND_MS 1000
#define MINUTE_MS ( SECOND * 60 )
#define HOUR_MS   ( SECOND * 3600 )

#define PRESENTING_DELAY_MS 16

#define BETWEEN_PICTURES_DELAY_MS 500

#define WATERSCREEN_CONFIG_GET_INTERVAL ( 3 * SECOND_MS )


typedef uint32_t delayMs_t;


#endif /* RTOS_TIME_DEFINES_H_ */
