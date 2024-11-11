#include "waterscreen_status.h"

#include "waterscreen_state/waterscreen_states.h"


WaterscreenStatus_t generateWaterscreenStatus( const WaterscreenContext_t *context )
{
    WaterscreenStatus_t status = { .mode         = Mode_Standard,
                                   .fluidLevel   = context->waterscreenStateHandler == lowWaterState ? FluidLevel_Low
                                                                                                     : FluidLevel_Optimal,
                                   .isPresenting = context->waterscreenStateHandler == presentationState };

    return status;
}
