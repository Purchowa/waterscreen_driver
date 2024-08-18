#include "validation.h"

#include "config/logging_config.h"
#include "wlan/wlan_mwm.h"
#include "waterscreen_states.h"

#include <fsl_common.h>
#include <stdint.h>

#define WATERSCREEN_STATE_COUNT 4
#define WLAN_STATE_COUNT        7

#define UNKNOWN_STATE "unknown"

typedef struct
{
    waterscreenStateFunction_t key;
    const char                *value;
} pair_t;

static const char *getCurrentStateName( const WaterscreenContext_t *context )
{
    static const pair_t statesMap[WATERSCREEN_STATE_COUNT] = { { .key = demoModeState, .value = "demo mode" },
                                                               { presentationState, "presentation" },
                                                               { idleState, "idle" },
                                                               { lowWaterState, "low water" } };

    for ( uint8_t i = 0; i < WATERSCREEN_STATE_COUNT; ++i )
    {
        if ( context->waterscreenStateHandler == statesMap[i].key )
            return statesMap[i].value;
    }

    return UNKNOWN_STATE;
}

void logWaterscreenStatus( const WaterscreenContext_t *context )
{
    if ( context->currentStateStatus == kStatus_Fail )
        LogError( "SPI transfer failed" );

    static waterscreenStateFunction_t previousState = NULL;
    if ( context->waterscreenStateHandler != previousState )
    {
        LogInfo( "Current water screen state: %s", getCurrentStateName( context ) );
        previousState = context->waterscreenStateHandler;
    }
}

void logWlanStatus()
{
    static int prevWlanState = -1;
    const int  wlanState     = wlan_get_state();


    if ( prevWlanState == wlanState )
        return;

    LogDebug( "Current MWM WLAN state: %d", wlanState );
    prevWlanState = wlanState;
}
