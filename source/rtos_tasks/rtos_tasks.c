#include "rtos_tasks.h"

#include "button_control.h"
#include "waterscreen_states.h"
#include "waterscreen_state_context.h"
#include "standard_mode_state.h"
#include "config/waterscreen_config.h"

#include "external_communication/weather_api.h"
#include "external_communication/datetime_api.h"
#include "external_communication/waterscreen_config_api.h"
#include "external_communication/wlan_adapter.h"
#include "status_logging.h"

#include "datetime/rtc_provider.h"
#include "logging_config.h"
#include "external/wlan/wlan_mwm.h"

#include <fsl_common.h>
#include <serial_mwm.h>


static WaterscreenContext_t s_context = { .waterscreenStateHandler         = idleState,
                                          .previousWaterscreenStateHandler = idleState,
                                          .pictureView                     = NULL,
                                          .valveOpenCounter                = 0,
                                          .currentStateStatus              = kStatus_Success,
                                          .currentStateDelay               = SECOND_MS };

static StandardModeConfig_t s_standardModeCfg = { .isWorkingDuringWeekends = true,
                                                  .workTimeInStandardMode  = 1,
                                                  .idleTimeInStandardMode  = 1,
                                                  .workRange               = { .from = 7, .to = 24 } };

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
        LogError( "Request for weather failed. Code: %d", weatherErrorCode );
    }
}

static void requestDatetime( Datetime_t *datetime )
{
    HttpReturnCodes_t datetimeErrorCode = httpGetDatetime( datetime );
    while ( datetimeErrorCode != Http_Success )
    {
        LogError( "Request for datetime failed. Code: %d", datetimeErrorCode );
        vTaskDelay( pdMS_TO_TICKS( 10 * SECOND_MS ) );

        datetimeErrorCode = httpGetDatetime( datetime );
    }
}

static void requestWaterscreenConfig( bool isInitialRequest )
{
    // TODO: workRange isn't provided from API hence it must be here as mocked value.
    static WaterscreenConfig_t waterscreenConfig = { .standardModeConfig = { .workRange = { .from = 7, .to = 24 } } };

    HttpReturnCodes_t cfgReturnCode = httpGetWaterscreenConfig( &waterscreenConfig, isInitialRequest );

    if ( cfgReturnCode == Http_Success )
    {
        LogInfo( "Water-screen configuration updated!" );

        forceChangeWaterscreenState( &s_context, g_waterscreenModes[waterscreenConfig.mode] );
        s_standardModeCfg = waterscreenConfig.standardModeConfig;
        initStandardModeConfig( &s_standardModeCfg );
    }
    else
    {
        LogInfo( "Water-screen configuration request. Code: %d", cfgReturnCode );
    }
}

static void handleRequests()
{
    assert( WATERSCREEN_CONFIG_GET_INTERVAL <= WEATHER_GET_INTERVAL );
    static uint32_t callCounter = 0;

    // TODO: Find better way... This is temporary solution for problems with inter-task w-lan requests.
    if ( WEATHER_GET_INTERVAL / WATERSCREEN_CONFIG_GET_INTERVAL <= callCounter++ )
    {
        const Datetime_t datetime = getRTCDatetime();
        if ( s_standardModeCfg.workRange.from <= datetime.time.hour &&
             datetime.time.hour <= s_standardModeCfg.workRange.to )
        {
            requestWeather();
        }

        callCounter = 0;
    }
    requestWaterscreenConfig( false );
}

void wifiTask( void * )
{
    Datetime_t datetime      = {};
    delayMs_t  requestsDelay = WATERSCREEN_CONFIG_GET_INTERVAL;

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

        vTaskDelay( pdMS_TO_TICKS( requestsDelay ) );
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
