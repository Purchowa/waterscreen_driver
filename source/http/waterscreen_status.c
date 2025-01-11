#include "waterscreen_status.h"

#include "waterscreen_state/standard_mode_state.h"
#include "waterscreen_state/waterscreen_states.h"


WaterscreenStatus_t generateWaterscreenStatus( const WaterscreenMode_t     currentMode,
                                               const WaterscreenContext_t *context )
{
    WaterscreenStatus_t status = { .mode         = currentMode,
                                   .fluidLevel   = context->waterscreenStateHandler == lowWaterState ? FluidLevel_Low
                                                                                                     : FluidLevel_Optimal,
                                   .isPresenting = context->waterscreenStateHandler == presentationState };

    return status;
}
