#ifndef GPIO_MOCKS_H
#define GPIO_MOCKS_H

#include "power_control.h"
#include "spi_transfer.h"
#include "sensors_control.h"
#include "spi_transfer.h"


void manageValvePower( DeviceState_t state );
void manageWaterPump( DeviceState_t state );

status_t sendDataToValves( const uint64_t *data );

bool shouldWaterPumpTrigger();
bool shouldWaterAlaramTrigger();

#endif // GPIO_MOCKS_H
