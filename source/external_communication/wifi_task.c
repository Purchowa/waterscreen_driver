#include "wifi_task.h"

#include "waterscreen_config.h"
#include "waterscreen_state/waterscreen_state_context.h"
#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/standard_mode_state.h"
#include "waterscreen_state/waterscreen_state_context_handler.h"

#include "datetime_api.h"
#include "weather_api.h"
#include "waterscreen_config_api.h"
#include "waterscreen_status_api.h"
#include "http_return_codes.h"
#include "wlan_adapter.h"

#include "datetime/rtc_provider.h"

#include "logging.h"
#include "status_logging.h"

#include <FreeRTOS.h>
#include <task.h>


static HttpReturnCodes_t s_httpReturnCode = Http_UnknownError;

static void requestWeather()
{
    s_httpReturnCode = httpGetWeather();

    logHttpRequest( "Weather", s_httpReturnCode, GET );
}

static void requestDatetime( Datetime_t *datetime )
{
    s_httpReturnCode = httpGetDatetime( datetime );
    while ( s_httpReturnCode != Http_Success )
    {
        logHttpRequest( "Date-time", s_httpReturnCode, GET );
        vTaskDelay( pdMS_TO_TICKS( 10 * SECOND_MS ) );

        s_httpReturnCode = httpGetDatetime( datetime );
    }
}

static void requestWaterscreenConfig( bool isInitialRequest )
{
    s_httpReturnCode = httpGetWaterscreenConfig( &g_waterscreenConfig, isInitialRequest );

    if ( s_httpReturnCode == Http_Success )
    {
        LogDebug( "WS-config updated!" );

        forceChangeWaterscreenState( &g_context, g_waterscreenModes[g_waterscreenConfig.mode.current] );
        initStandardModeConfig( &g_waterscreenConfig.standardModeConfig );
    }
    else
    {
        logHttpRequest( "WS-config", s_httpReturnCode, GET );
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
        if ( g_waterscreenConfig.standardModeConfig.workRange.from <= datetime.time.hour &&
             datetime.time.hour <= g_waterscreenConfig.standardModeConfig.workRange.to )
        {
            requestWeather();
        }
    }

    if ( callCounter % WATERSCREEN_STATE_POST_CALLS_NUMBER == 0 )
    {
        const WaterscreenStatus_t status = generateWaterscreenStatus( g_waterscreenConfig.mode.current, &g_context );
        s_httpReturnCode                 = httpPostWaterscreenStatus( &status );

        logHttpRequest( "WS-status", s_httpReturnCode, POST );
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

HttpReturnCodes_t getLastHttpCode()
{
    return s_httpReturnCode;
}
