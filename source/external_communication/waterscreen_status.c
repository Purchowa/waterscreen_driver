#include "waterscreen_status.h"

#include "waterscreen_state/standard_mode_state.h"
#include "waterscreen_state/waterscreen_states.h"


WaterscreenMode_t mapStatesToMode( const WaterscreenContext_t *context )
{
    static WaterscreenMode_t mode = Mode_Standard;

    const waterscreenStateFunction_t prevState = context->previousWaterscreenStateHandler;
    const waterscreenStateFunction_t currState = context->waterscreenStateHandler;

    if ( prevState == standardModeState || currState == standardModeState )
        mode = Mode_Standard;

    if ( prevState == demoModeState || currState == demoModeState )
        mode = Mode_Demo;

    if ( prevState == idleState || currState == idleState ) // TODO: change to service
        mode = Mode_Service;

    return mode;
}

WaterscreenStatus_t generateWaterscreenStatus( const WaterscreenContext_t *context )
{
    WaterscreenStatus_t status = { .mode         = mapStatesToMode( context ),
                                   .fluidLevel   = context->waterscreenStateHandler == lowWaterState ? FluidLevel_Low
                                                                                                     : FluidLevel_Optimal,
                                   .isPresenting = context->waterscreenStateHandler == presentationState };

    return status;
}
