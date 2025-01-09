#include "receiver_handlers.h"

#include "config/waterscreen_config.h"
#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/realtime_mode.h"
#include "waterscreen_state/waterscreen_state_context_handler.h"

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
