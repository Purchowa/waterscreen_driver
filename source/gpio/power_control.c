#include "power_control.h"

#include "fsl_device_registers.h"
#include "fsl_gpio.h"
#include "pin_mux.h"
#include "status_logging.h"

static DeviceState_t s_valvePowerState = OffDeviceState;
static DeviceState_t s_waterPumpState  = OffDeviceState;

void manageValvePower( DeviceState_t state )
{
    GPIO_PinWrite( BOARD_INITGPIOPINS_POWER_SUPPLY_GPIO, BOARD_INITGPIOPINS_POWER_SUPPLY_PORT,
                   BOARD_INITGPIOPINS_POWER_SUPPLY_PIN, state );

    s_valvePowerState = state;
    logValvePowerState( state );
}

void manageWaterPump( DeviceState_t state )
{
    GPIO_PinWrite( BOARD_INITGPIOPINS_PUMP_GPIO, BOARD_INITGPIOPINS_PUMP_PORT, BOARD_INITGPIOPINS_PUMP_PIN, state );

    s_waterPumpState = state;
    logWaterPumpState( state );
}

DeviceState_t getValvePowerState()
{
    return s_valvePowerState;
}

DeviceState_t getWaterPumpState()
{
    return s_waterPumpState;
}

const char *getDeviceStateStr( DeviceState_t state )
{
    return state == OffDeviceState ? "off" : "on";
}
