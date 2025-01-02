#include "realtime_mode.h"

#include "picture_management/picture_data.h"
#include "picture_management/picture_logic_utils.h"

#include "gpio/power_control.h"

#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/waterscreen_state_context_handler.h"


void realtimeModeState( WaterscreenContext_t *context )
{
    context->pictureInfo      = &g_customPicture;
    context->valveOpenCounter = getLastPictureIndex( &context->pictureInfo->picture );

    manageValvePower( OnDeviceState );
    changeWaterscreenState( context, presentationState );
}
