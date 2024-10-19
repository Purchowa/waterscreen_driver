#include "waterscreen_state_context.h"

#include "gpio/power_control.h"
#include "waterscreen_states.h"


void changeWaterscreenState( WaterscreenContext_t *context, waterscreenStateFunction_t newState )
{
    context->previousWaterscreenStateHandler = context->waterscreenStateHandler;
    context->waterscreenStateHandler         = newState;
}

void forceChangeWaterscreenState( WaterscreenContext_t *context, waterscreenStateFunction_t newState )
{
    manageValvePower( OffDeviceState );
    closeValvesSubState( context );
    changeWaterscreenState( context, newState );
}

void performWaterscreenAction( WaterscreenContext_t *context )
{
    context->waterscreenStateHandler( context );
}

void goBackToPreviousWaterscreenState( WaterscreenContext_t *context )
{
    context->waterscreenStateHandler = context->previousWaterscreenStateHandler;
}
