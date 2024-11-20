#include "status_logging.h"

#include "waterscreen_state_context.h"
#include "external_communication/weather_types.h"
#include "datetime/datetime_types.h"

#include "config/logging_config.h"
#include "wlan/wlan_mwm.h"
#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/standard_mode_state.h"
#include "waterscreen_state/state_utils.h"

#include <fsl_common.h>
#include <stdint.h>


void logWaterscreenStatus( const WaterscreenContext_t *context )
{
    if ( context->currentStateStatus == kStatus_Fail )
        LogError( "SPI transfer failed" );

    static waterscreenStateFunction_t previousState = NULL;
    if ( context->waterscreenStateHandler != previousState )
    {
        LogDebug( "Current water screen state: %s", getStateName( context->waterscreenStateHandler ) );
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

void logValvePowerState( DeviceState_t state )
{
    static DeviceState_t recentState = OffDeviceState;

    if ( recentState != state )
    {
        LogDebug( "[GPIO] Valve power %s", state == OffDeviceState ? "off" : "on" );
        recentState = state;
    }
}

void logWaterPumpState( DeviceState_t state )
{
    static DeviceState_t recentState = OffDeviceState;

    if ( recentState != state )
    {
        LogDebug( "[GPIO] Water pump %s", state == OffDeviceState ? "off" : "on" );
        recentState = state;
    }
}
