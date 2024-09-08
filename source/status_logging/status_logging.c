#include <logging.h>

#include "waterscreen_state_context.h"
#include "external_communication/weather_types.h"
#include "datetime/datetime_types.h"

#include "config/logging_config.h"
#include "wlan/wlan_mwm.h"
#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/standard_mode_state.h"

#include <fsl_common.h>
#include <stdint.h>

#define WATERSCREEN_STATE_COUNT 5

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
                                                               { lowWaterState, "low water" },
                                                               { standardModeState, "standard mode" } };

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


void logWeather( const Weather_t *weather )
{
    LogInfo( "Weather: condition - %d, temperature - %.2f C, pressure - %d hPa", weather->weatherCondition,
             weather->temperature, weather->pressure );
}

void logDatetime( const Datetime_t *datetime )
{
    LogInfo( "Time: weekday - %d, date: %d.%d.%d time: %d:%d:%d", datetime->date.weekday, datetime->date.day,
             datetime->date.month, datetime->date.year, datetime->time.hour, datetime->time.minute,
             datetime->time.second );
}
