#ifndef STATUS_LOGGING_H_
#define STATUS_LOGGING_H_

#include "datetime/datetime_types.h"
#include "external_communication/weather_types.h"
#include "external_communication/http_return_codes.h"
#include "waterscreen_state/waterscreen_state_context.h"
#include "gpio/power_control.h"

void logWaterscreenStatus( const WaterscreenContext_t *context );
void logWlanStatus();
void logWeather( const Weather_t *weather );
void logDatetime( const Datetime_t *datetime );
void logValvePowerState( DeviceState_t state );
void logWaterPumpState( DeviceState_t state );

typedef enum
{
    GET,
    POST
} RequestMethod_t;

void logHttpRequest( const char *requestTargetName, const HttpReturnCodes_t responseCode,
                     const RequestMethod_t requestMethod );

#endif /* STATUS_LOGGING_H_ */
