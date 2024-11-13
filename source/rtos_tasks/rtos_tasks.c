#include "rtos_tasks.h"

#include "button_control.h"
#include "waterscreen_states.h"
#include "waterscreen_state_context.h"
#include "standard_mode_state.h"
#include "config/waterscreen_config.h"

#include "external_communication/weather_api.h"
#include "external_communication/datetime_api.h"
#include "external_communication/waterscreen_config_api.h"
#include "external_communication/waterscreen_status.h"
#include "external_communication/waterscreen_status_api.h"
#include "external_communication/wlan_adapter.h"
#include "status_logging.h"

#include "datetime/rtc_provider.h"
#include "external/wlan/wlan_mwm.h"
#include "logging_config.h"

#include <fsl_common.h>


static WaterscreenContext_t s_context = { .waterscreenStateHandler         = idleState,
                                          .previousWaterscreenStateHandler = idleState,
                                          .picture                         = NULL,
                                          .valveOpenCounter                = 0,
                                          .currentStateStatus              = kStatus_Success,
                                          .currentStateDelay               = SECOND_MS };

static StandardModeConfig_t s_standardModeCfg = { .isWorkingDuringWeekends = false,
                                                  .workTimeInStandardMode  = 5,
                                                  .idleTimeInStandardMode  = 5,
                                                  .workRange               = { .from = 7, .to = 18 } };

void hmiTask( void *params )
{
    for ( ;; )
    {
        // USER
        if ( isS3ButtonPressed() )
        {
            forceChangeWaterscreenState( &s_context, demoModeState );
        }

        // WAKEUP
        if ( isS2ButtonPressed() )
        {
            forceChangeWaterscreenState( &s_context, standardModeState );
        }

        // ISP
        if ( isS1ButtonPressed() )
        {
            forceChangeWaterscreenState( &s_context, idleState );
        }
    }
}

static void requestWeather()
{
    HttpReturnCodes_t weatherErrorCode = httpGetWeather();
    if ( weatherErrorCode != Http_Success )
    {
        LogError( "GET request for weather failed. Code: %d", weatherErrorCode );
    }
}

static void requestDatetime( Datetime_t *datetime )
{
    HttpReturnCodes_t datetimeErrorCode = httpGetDatetime( datetime );
    while ( datetimeErrorCode != Http_Success )
    {
        LogError( "GET request for datetime failed. Code: %d", datetimeErrorCode );
        vTaskDelay( pdMS_TO_TICKS( 10 * SECOND_MS ) );

        datetimeErrorCode = httpGetDatetime( datetime );
    }
}

static void requestWaterscreenConfig( bool isInitialRequest )
{
    static WaterscreenConfig_t waterscreenConfig;

    HttpReturnCodes_t cfgReturnCode = httpGetWaterscreenConfig( &waterscreenConfig, isInitialRequest );

    if ( cfgReturnCode == Http_Success )
    {
        LogDebug( "Water-screen configuration updated!" );

        forceChangeWaterscreenState( &s_context, g_waterscreenModes[waterscreenConfig.mode] );
        s_standardModeCfg = waterscreenConfig.standardModeConfig;
        initStandardModeConfig( &s_standardModeCfg );
    }
    else
    {
        LogDebug( "Water-screen configuration GET request. Code: %d", cfgReturnCode );
    }
}


static void handleRequests()
{
    static uint32_t callCounter = 0;

    if ( callCounter % WATERSCREEN_CONFIG_GET_CALLS_NUMBER == 0 )
    {
        requestWaterscreenConfig( false );
    }

    if ( callCounter % WEATHER_GET_CALLS_NUMBER == 0 )
    {
        const Datetime_t datetime = getRTCDatetime();
        if ( s_standardModeCfg.workRange.from <= datetime.time.hour &&
             datetime.time.hour <= s_standardModeCfg.workRange.to )
        {
            requestWeather();
        }
    }

    if ( callCounter % WATERSCREEN_STATE_POST_CALLS_NUMBER == 0 )
    {
        const WaterscreenStatus_t status     = generateWaterscreenStatus( &s_context );
        HttpReturnCodes_t         returnCode = httpPostWaterscreenStatus( &status );

        LogDebug( "Water-screen status POST request. Code: %d", returnCode );
    }

    ++callCounter; // no overflow for unsigned it's just modulo
}

void wifiTask( void * )
{
    Datetime_t datetime = {};

    initSerialMWM();

    requestWeather();
    requestDatetime( &datetime );
    requestWaterscreenConfig( true );

    setRTCDatetime( &datetime );

    logDatetime( &datetime );
    logWeather( getWeather() );

    for ( ;; )
    {
        logWlanStatus();
        handleRequests();

        vTaskDelay( pdMS_TO_TICKS( WIFI_TASK_DELAY_MS ) );
    }
}

void waterscreenActionTask( void *params )
{
    initStandardModeConfig( &s_standardModeCfg );

    TickType_t lastWakeTime = xTaskGetTickCount();
    for ( ;; )
    {
        performWaterscreenAction( &s_context );
        logWaterscreenStatus( &s_context );

        vTaskDelayUntil( &lastWakeTime, pdMS_TO_TICKS( s_context.currentStateDelay ) );
    }
}
