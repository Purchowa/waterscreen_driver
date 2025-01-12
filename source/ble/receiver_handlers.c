#include "receiver_handlers.h"

#include "config/waterscreen_config.h"
#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/realtime_mode.h"
#include "waterscreen_state/waterscreen_state_context_handler.h"
#include "waterscreen_state/standard_mode_state.h"

#include "common_utils.h"

#include <string.h>


#define STRINGS_EQUAL 0

#define CLIENT_CONNECTED_EVN   "OK+CONN"
#define CLIENT_CONNECTION_LOST "OK+LOST"

void handleBLENotifyEvents( const char *event, bool *isClientConnected )
{
    if ( strcmp( event, CLIENT_CONNECTED_EVN ) == STRINGS_EQUAL )
    {
        *isClientConnected = true;
    }
    else if ( strcmp( event, CLIENT_CONNECTION_LOST ) == STRINGS_EQUAL )
    {
        *isClientConnected = false;
    }
}

void handleBLERTModeEvent( bool isRTModeActive )
{
    if ( isRTModeActive && g_waterscreenConfig.mode.current != Mode_RT )
    {
        setWaterscreenMode( &g_waterscreenConfig.mode, Mode_RT );
        forceChangeWaterscreenState( &g_context, idleState );
    }
    else if ( !isRTModeActive && g_waterscreenConfig.mode.current == Mode_RT )
    {
        setWaterscreenMode( &g_waterscreenConfig.mode, g_waterscreenConfig.mode.previous );
        forceChangeWaterscreenState( &g_context, g_waterscreenConfigAvailableModes[g_waterscreenConfig.mode.current] );
    }
}

void handleCustomPictureEvent()
{
    if ( g_waterscreenConfig.mode.current == Mode_RT )
    {
        setRtPicturePresentationRepeatCount( 3 );
        forceChangeWaterscreenState( &g_context, realtimeModeState );
    }
}

static void deserializeConfiguration( const SerializedConfiguration_t *src, WaterscreenConfig_t *dst )
{
    setWaterscreenMode( &dst->mode, clamp( src->waterscreenMode, 0, CONFIG_AVAILABLE_MODE_COUNT - 1 ) );

    StandardModeConfig_t *standardCfg    = &dst->standardModeConfig;
    standardCfg->isWorkingDuringWeekends = src->isWorkingDuringWeekends;
    standardCfg->workTimeInStandardMode  = clamp( src->workTimeInStandardMode, 0, STANDARD_MAX_WORK_TIME );
    standardCfg->idleTimeInStandardMode  = clamp( src->idleTimeInStandardMode, 0, STANDARD_MAX_IDLE_TIME );
    standardCfg->workRange.to            = clamp( src->workTo, 0, STANDARD_MAX_WORK_RANGE_VALUE );
    standardCfg->workRange.from          = clamp( src->workFrom, 0, src->workTo ); // work range -> [from; to]
}

void handleConfiguration( const SerializedConfiguration_t *serializedConfig )
{
    deserializeConfiguration( serializedConfig, &g_waterscreenConfig );

    forceChangeWaterscreenState( &g_context, g_waterscreenConfigAvailableModes[g_waterscreenConfig.mode.current] );
    initStandardModeConfig( &g_waterscreenConfig.standardModeConfig );
}
