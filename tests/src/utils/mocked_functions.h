#ifndef MOCKED_FUNCTIONS_H_
#define MOCKED_FUNCTIONS_H_

#include "power_control.h"
#include "spi_transfer.h"
#include "waterscreen_state_context.h"
#include "pictures.h"
#include "rtc_provider.h"
#include "weather_api.h"

/* Redeclarations for better redability */

void manageValvePower( DeviceState_t state );
void manageWaterPump( DeviceState_t state );

status_t sendDataToValves( const uint64_t *data );

bool shouldWaterPumpTrigger();
bool shouldWaterAlaramTrigger();

const pictureDataView_t *getEachPictureView();

void       will_return_datetime( const Datetime_t datetime );
Datetime_t getRTCDatetime();

WeatherApiStatusCode_t getWeather( Weather_t *weather );

#endif
