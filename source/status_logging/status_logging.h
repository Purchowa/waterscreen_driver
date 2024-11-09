#ifndef STATUS_LOGGING_H_
#define STATUS_LOGGING_H_

#include "datetime/datetime_types.h"
#include "external_communication/weather_types.h"
#include "waterscreen_state/waterscreen_state_context.h"
#include "gpio/power_control.h"

void logWaterscreenStatus( const WaterscreenContext_t *context );
void logWlanStatus();
void logWeather( const Weather_t *weather );
void logDatetime( const Datetime_t *datetime );
void logValvePowerState( DeviceState_t state );
void logWaterPumpState( DeviceState_t state );


#endif /* STATUS_LOGGING_H_ */
