#ifndef STATUS_LOGGING_H_
#define STATUS_LOGGING_H_

#include "datetime/datetime_types.h"
#include "waterscreen_state/waterscreen_state_context.h"

void logWaterscreenStatus( const WaterscreenContext_t *context );
void logWlanStatus();
void logWeather( const Weather_t *weather );
void logDatetime( const Datetime_t *datetime );


#endif /* STATUS_LOGGING_H_ */
