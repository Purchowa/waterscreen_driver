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
#include "oled_screen/oled_panel.h"

#include "datetime/rtc_provider.h"
#include "external/wlan/wlan_mwm.h"
#include "logging_config.h"

#include <fsl_common.h>


static WaterscreenContext_t s_context = { .waterscreenStateHandler         = standardModeState,
                                          .previousWaterscreenStateHandler = demoModeState,
                                          .pictureInfo                     = NULL,
                                          .valveOpenCounter                = 0,
                                          .currentStateStatus              = kStatus_Success,
                                          .currentStateDelay               = SECOND_MS };

static WaterscreenConfig_t s_waterscreenConfig = {
    .mode               = Mode_Standard,
    .standardModeConfig = { .isWorkingDuringWeekends = false,
                            .workTimeInStandardMode  = 5,
                            .idleTimeInStandardMode  = 5,
                            .workRange               = { .from = 7, .to = 18 } },
    .customPicture      = &g_customPicture,
};

static HttpReturnCodes_t s_httpReturnCode = Http_UnknownError;

void hmiTask( void *params )
{
    for ( ;; )
    {
        drawInfoPanel( &s_context, s_waterscreenConfig.mode, s_httpReturnCode );

        // USER
        if ( isS3ButtonPressed() )
        {
            forceChangeWaterscreenState( &s_context, demoModeState );
            s_waterscreenConfig.mode = Mode_Demo;
        }

        // WAKEUP
        if ( isS2ButtonPressed() )
        {
            forceChangeWaterscreenState( &s_context, standardModeState );
            s_waterscreenConfig.mode = Mode_Standard;
        }

        // ISP
        if ( isS1ButtonPressed() )
        {
            forceChangeWaterscreenState( &s_context, idleState );
            s_waterscreenConfig.mode = Mode_Service;
        }

        vTaskDelay( pdMS_TO_TICKS( 32 ) );
    }
}

static void requestWeather()
{
    s_httpReturnCode = httpGetWeather();

    LogDebug( "Weather GET request. Code: %d", s_httpReturnCode );
}

static void requestDatetime( Datetime_t *datetime )
{
    s_httpReturnCode = httpGetDatetime( datetime );
    while ( s_httpReturnCode != Http_Success )
    {
        LogError( "GET request for date-time failed. Code: %d", s_httpReturnCode );
        vTaskDelay( pdMS_TO_TICKS( 10 * SECOND_MS ) );

        s_httpReturnCode = httpGetDatetime( datetime );
    }
}

static void requestWaterscreenConfig( bool isInitialRequest )
{
    s_httpReturnCode = httpGetWaterscreenConfig( &s_waterscreenConfig, isInitialRequest );

    if ( s_httpReturnCode == Http_Success )
    {
        LogDebug( "Water-screen configuration updated!" );

        forceChangeWaterscreenState( &s_context, g_waterscreenModes[s_waterscreenConfig.mode] );
        initStandardModeConfig( &s_waterscreenConfig.standardModeConfig );
    }
    else
    {
        LogDebug( "Water-screen configuration GET request. Code: %d", s_httpReturnCode );
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
        if ( s_waterscreenConfig.standardModeConfig.workRange.from <= datetime.time.hour &&
             datetime.time.hour <= s_waterscreenConfig.standardModeConfig.workRange.to )
        {
            requestWeather();
        }
    }

    if ( callCounter % WATERSCREEN_STATE_POST_CALLS_NUMBER == 0 )
    {
        const WaterscreenStatus_t status = generateWaterscreenStatus( s_waterscreenConfig.mode, &s_context );
        s_httpReturnCode                 = httpPostWaterscreenStatus( &status );

        LogDebug( "Water-screen status POST request. Code: %d", s_httpReturnCode );
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
    initStandardModeConfig( &s_waterscreenConfig.standardModeConfig );

    TickType_t lastWakeTime = xTaskGetTickCount();
    for ( ;; )
    {
        performWaterscreenAction( &s_context );
        logWaterscreenStatus( &s_context );

        vTaskDelayUntil( &lastWakeTime, pdMS_TO_TICKS( s_context.currentStateDelay ) );
    }
}
