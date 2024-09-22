#ifndef MOCKED_FUNCTIONS_H_
#define MOCKED_FUNCTIONS_H_

#include "power_control.h"
#include "spi_transfer.h"
#include "waterscreen_state_context.h"
#include "picture_managment/demo_mode_picture_logic.h"
#include "weather_api.h"

/* Redeclarations for better redability */

void manageValvePower( DeviceState_t state );
void manageWaterPump( DeviceState_t state );

status_t sendDataToValves( const uint64_t *data );

bool shouldWaterPumpTrigger();
bool shouldWaterAlaramTrigger();

const PictureDataView_t *getEachPictureView();
const PictureDataView_t *getOccasionalPictureView( const Datetime_t *, const WeatherCondition_t );

void       will_return_datetime( const Datetime_t datetime );
Datetime_t getRTCDatetime();

WeatherApiStatusCode_t getWeather( Weather_t *weather );

#endif
