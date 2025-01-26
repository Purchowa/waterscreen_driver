#include "realtime_mode.h"

#include "picture_management/picture_data.h"
#include "picture_management/picture_logic_utils.h"

#include "gpio/power_control.h"

#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/waterscreen_state_context_handler.h"

static uint8_t s_presentationRepeatCount = 0;

void setRtPicturePresentationRepeatCount( uint8_t n )
{
    s_presentationRepeatCount = n;
}

void realtimeModeState( WaterscreenContext_t *context )
{
    if ( s_presentationRepeatCount != 0 )
    {
        context->pictureInfo      = &g_customPicture;
        context->valveOpenCounter = getLastPictureIndex( &context->pictureInfo->picture );

        manageValvePower( OnDeviceState );
        changeWaterscreenState( context, presentationState );
        context->stateDelay = PRESENTING_DELAY_MS;
        --s_presentationRepeatCount;
    }
    else
    {
        manageValvePower( OffDeviceState );
        changeWaterscreenState( context, idleState );
    }
}
