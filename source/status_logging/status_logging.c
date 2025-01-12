#include "status_logging.h"

#include "waterscreen_state_context.h"
#include "http/weather_types.h"
#include "datetime/datetime_types.h"

#include "logging.h"
#include "wlan/wlan_mwm.h"
#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/standard_mode_state.h"
#include "waterscreen_state/state_utils.h"
#include "gpio/button_control.h"

#include "ble/ble_send_provider.h"

#include <inttypes.h>


void logWaterscreenStatus( const WaterscreenContext_t *context )
{
    if ( context->stateStatus == kStatus_Fail )
        LogError( "SPI transfer failed" );

    static waterscreenStateFunction_t previousState = NULL;
    if ( context->waterscreenStateHandler != previousState )
    {
        LogDebug( "State: %s", getStateName( context->waterscreenStateHandler ) );
        previousState = context->waterscreenStateHandler;
    }
}

void logWlanStatus()
{
    static int prevWlanState = -1;
    const int  wlanState     = wlan_get_state();

    if ( prevWlanState == wlanState )
        return;

    LogDebug( "MWM WLAN state: %d", wlanState );
    prevWlanState = wlanState;
}


void logWeather( const Weather_t *weather )
{
    LogInfo( "Weather - condition: %d, temperature: %.2f C, pressure: %d hPa", weather->weatherCondition,
             weather->temperature, weather->pressure );
}

void logDatetime( const Datetime_t *datetime )
{
    LogInfo( "Time - weekday: %d, date: %d.%d.%d time: %d:%d:%d", datetime->date.weekday, datetime->date.day,
             datetime->date.month, datetime->date.year, datetime->time.hour, datetime->time.minute,
             datetime->time.second );
}

void logValvePowerState( DeviceState_t state )
{
    static DeviceState_t recentState = OffDeviceState;

    if ( recentState != state )
    {
        LogDebug( "[GPIO] Valve power %s", getDeviceStateStr( state ) );
        recentState = state;
    }
}

void logWaterPumpState( DeviceState_t state )
{
    static DeviceState_t recentState = OffDeviceState;

    if ( recentState != state )
    {
        LogDebug( "[GPIO] Water pump %s", getDeviceStateStr( state ) );
        recentState = state;
    }
}

void logWaterscreenConfig( const WaterscreenConfig_t *config )
{
    LogDebug( "[Configuration]\nMode: %s\nIsWorkingDuringWeekend: %" PRIu8 "\nWorkTime: %" PRIu16
              "m\nIdleTime: %" PRIu16 "m\nWork - from: %" PRIu8 ", to: %" PRIu8 "",
              getModeName( config->mode.current ), config->standardModeConfig.isWorkingDuringWeekends,
              config->standardModeConfig.workTimeInStandardMode, config->standardModeConfig.idleTimeInStandardMode,
              config->standardModeConfig.workRange.from, config->standardModeConfig.workRange.to );
}

static const char *s_requestMethodsNames[] = { "GET", "POST" };

void logHttpRequest( const char *requestTargetName, const HttpReturnCodes_t responseCode,
                     const RequestMethod_t requestMethod )
{
    LogDebug( "%s %s request. Response: [%d]-%s", requestTargetName, s_requestMethodsNames[requestMethod], responseCode,
              getHttpReturnCodeName( responseCode ) );
}
