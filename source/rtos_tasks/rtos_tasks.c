#include "rtos_tasks.h"

#include "button_control.h"
#include "waterscreen_states.h"
#include "waterscreen_state_context.h"
#include "standard_mode_state.h"
#include "config/waterscreen_config.h"

#include "external_communication/weather_api.h"
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

static Weather_t s_weather;

static Datetime_t s_datetime;

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
    HttpReturnCodes_t weatherErrorCode = getWeather( &s_weather );
    while ( weatherErrorCode != Http_Success )
    {
        LogError( "Request for weather failed. Code: %d", weatherErrorCode );
        vTaskDelay( pdMS_TO_TICKS( 10 * SECOND_MS ) );

        weatherErrorCode = getWeather( &s_weather );
    }
}

static void requestDatetime()
{
    HttpReturnCodes_t datetimeErrorCode = getDatetime( &s_datetime );
    while ( datetimeErrorCode != Http_Success )
    {
        LogError( "Request for datetime failed. Code: %d", datetimeErrorCode );
        vTaskDelay( pdMS_TO_TICKS( 10 * SECOND_MS ) );

        datetimeErrorCode = getDatetime( &s_datetime );
    }
}

static void requestWaterscreenConfig( bool isInitialRequest )
{
    static WaterscreenConfig_t waterscreenConfig = {};

    HttpReturnCodes_t cfgReturnCode = getWaterscreenConfigFromApi( &waterscreenConfig, isInitialRequest );
    switch ( cfgReturnCode )
    {
    case Http_Success:
        {
            LogInfo( "Water-screen configuration updated!" );

            forceChangeWaterscreenState( &s_context, g_waterscreenModes[waterscreenConfig.mode] );
            s_standardModeCfg = waterscreenConfig.standardModeConfig;
            initStandardModeConfig( &s_standardModeCfg );
            break;
        }
    default:
        LogInfo( "Water-screen configuration request. Code: %d", cfgReturnCode );
    }
}

void wifiTask( void * )
{
    initSerialMWM();

    requestWeather();
    requestDatetime();
    requestWaterscreenConfig( true );

    setRTCDatetime( &s_datetime );

    logWeather( &s_weather );
    logDatetime( &s_datetime );


    for ( ;; )
    {
        logWlanStatus();
        requestWaterscreenConfig( false );

        vTaskDelay( pdMS_TO_TICKS( 5 * SECOND_MS ) );
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
