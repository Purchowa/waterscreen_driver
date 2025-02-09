#ifndef STATUS_LOGGING_H_
#define STATUS_LOGGING_H_

#include "datetime/datetime_types.h"
#include "http/weather_types.h"
#include "http/http_return_codes.h"
#include "waterscreen_state/waterscreen_state_context.h"
#include "gpio/power_control.h"
#include "config/waterscreen_config.h"

void logWaterscreenStatus( const WaterscreenContext_t *context );
void logWlanStatus();
void logWeather( const Weather_t *weather );
void logDatetime( const Datetime_t *datetime );
void logValvePowerState( DeviceState_t state );
void logWaterPumpState( DeviceState_t state );
void logWaterscreenConfig( const WaterscreenConfig_t *config );

typedef enum
{
    GET,
    POST
} RequestMethod_t;

void logHttpRequest( const char *requestTargetName, const HttpReturnCodes_t responseCode,
                     const RequestMethod_t requestMethod );

#endif /* STATUS_LOGGING_H_ */
