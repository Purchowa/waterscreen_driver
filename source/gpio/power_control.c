#include "power_control.h"

#include "fsl_device_registers.h"
#include "fsl_gpio.h"
#include "pin_mux.h"

void manageValvePower( DeviceState_t state )
{

    GPIO_PinWrite( BOARD_INITGPIOPINS_POWER_SUPPLY_GPIO, BOARD_INITGPIOPINS_POWER_SUPPLY_PORT,
                   BOARD_INITGPIOPINS_POWER_SUPPLY_PIN, state );
}

void manageWaterPump( DeviceState_t state )
{

    GPIO_PinWrite( BOARD_INITGPIOPINS_PUMP_GPIO, BOARD_INITGPIOPINS_PUMP_PORT, BOARD_INITGPIOPINS_PUMP_PIN, state );
}
