#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

void changeWaterscreenState( WaterscreenContext_t *context, waterscreenStateFunction_t newState )
{
    context->waterscreenStateHandler = newState;
}

void performWaterscreenAction( WaterscreenContext_t *context )
{
    context->waterscreenStateHandler( context );
}
