#include "wifi_task.h"

#include "config/waterscreen_config.h"
#include "config/wifi_cfg.h"

#include "waterscreen_state/waterscreen_state_context.h"
#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/standard_mode_state.h"
#include "waterscreen_state/waterscreen_state_context_handler.h"

#include "datetime_api.h"
#include "weather_api.h"
#include "waterscreen_config_api.h"
#include "waterscreen_status_api.h"
#include "webpicture_api.h"
#include "http_return_codes.h"

#include "datetime/rtc_provider.h"

#include "logging.h"
#include "status_logging.h"
#include "external/wlan/wlan_mwm.h"

#include <serial_mwm.h>
#include <FreeRTOS.h>
#include <task.h>


static HttpReturnCodes_t s_httpReturnCode  = Http_UnknownError;
static bool              s_reconfigureWiFi = false;

static void requestWeather()
{
    s_httpReturnCode = httpGetWeather();

    logHttpRequest( "Weather", s_httpReturnCode, GET );
}

static void requestCustomPicture( bool isInitialRequest )
{
    s_httpReturnCode = httpGetCustomPicture( &g_customPicture, isInitialRequest );

    logHttpRequest( "WebPicture", s_httpReturnCode, GET );
}

static void requestDatetime( Datetime_t *datetime )
{
    s_httpReturnCode = httpGetDatetime( datetime );

    logHttpRequest( "Date-time", s_httpReturnCode, GET );
}

static void requestWaterscreenConfig( bool isInitialRequest )
{
    s_httpReturnCode = httpGetWaterscreenConfig( &g_waterscreenConfig, isInitialRequest );

    if ( s_httpReturnCode == Http_Success )
    {
        logWaterscreenConfig( &g_waterscreenConfig );

        forceChangeWaterscreenState( &g_context, g_waterscreenConfigAvailableModes[g_waterscreenConfig.mode.current] );
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
        requestCustomPicture( false );
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

    if ( callCounter % DATETIME_GET_CALLS_NUMBER == 0 )
    {
        Datetime_t datetime = {};

        requestDatetime( &datetime );
        setRTCDatetime( &datetime );
        logDatetime( &datetime );
    }

    ++callCounter; // no overflow for unsigned it's just modulo
}

void wifiTask( void * )
{
    Datetime_t datetime = {};

    wlan_init( g_wifiCredentials.login, g_wifiCredentials.password, AP_SECURITY_MODE );

    requestWeather();
    requestDatetime( &datetime );
    requestWaterscreenConfig( true );
    requestCustomPicture( true );

    setRTCDatetime( &datetime );

    logDatetime( &datetime );
    logWeather( getWeather() );

    for ( ;; )
    {
        logWlanStatus();
        if ( wlan_get_state() == MWM_CONNECTED )
        {
            handleRequests();
        }

        if ( s_reconfigureWiFi )
        {
            wlan_config( g_wifiCredentials.login, g_wifiCredentials.password, AP_SECURITY_MODE );
            s_reconfigureWiFi = false;
        }

        vTaskDelay( pdMS_TO_TICKS( WIFI_TASK_DELAY_MS ) );
    }
}

HttpReturnCodes_t getLastHttpCode()
{
    return s_httpReturnCode;
}

void reconfigureWifi()
{
    s_reconfigureWiFi = true;
}
