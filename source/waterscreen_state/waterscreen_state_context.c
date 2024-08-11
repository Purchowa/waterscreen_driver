#include "waterscreen_state_context.h"

void changeWaterscreenState( WaterscreenContext_t *context, waterscreenStateFunction_t newState )
{
    context->previousWaterscreenStateHandler = context->waterscreenStateHandler;
    context->waterscreenStateHandler         = newState;
}

void performWaterscreenAction( WaterscreenContext_t *context )
{
    context->waterscreenStateHandler( context );
}

void goBackToPreviousWaterscreenState( WaterscreenContext_t *context )
{
    context->waterscreenStateHandler = context->previousWaterscreenStateHandler;
}
