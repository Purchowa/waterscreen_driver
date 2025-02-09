#include "waterscreen_state_context_handler.h"

#include "waterscreen_states.h"
#include "gpio/power_control.h"
#include "neopixels/neopixel_provider.h"


void changeWaterscreenState( WaterscreenContext_t *context, waterscreenStateFunction_t newState )
{
    context->previousWaterscreenStateHandler = context->waterscreenStateHandler;
    context->waterscreenStateHandler         = newState;
}

void forceChangeWaterscreenState( WaterscreenContext_t *context, waterscreenStateFunction_t newState )
{
    changeWaterscreenState( context, newState );
    dimNeopixels();
    shutdownValves( context );
}

void performWaterscreenAction( WaterscreenContext_t *context )
{
    context->waterscreenStateHandler( context );
}

void goBackToPreviousWaterscreenState( WaterscreenContext_t *context )
{
    context->waterscreenStateHandler = context->previousWaterscreenStateHandler;
}
