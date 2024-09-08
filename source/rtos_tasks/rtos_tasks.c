#include "rtos_tasks.h"

#include "button_control.h"
#include "waterscreen_states.h"
#include "waterscreen_state_context.h"
#include "config/waterscreen_config.h"

#include "external_communication/weather_api.h"
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
                                          .currentStateStatus              = kStatus_Success };

static Weather_t s_weather;

static Datetime_t s_datetime;


void hmiTask( void *params )
{
    for ( ;; )
    {
        if ( isS3ButtonPressed() && s_context.waterscreenStateHandler == idleState )
        {
            forceChangeWaterscreenState( &s_context, demoModeState );
        }

        if ( isS2ButtonPressed() )
        {
            forceChangeWaterscreenState( &s_context, idleState );
        }
    }
    vTaskSuspend( NULL ); // Basically kill task.
}

static void requestWeather()
{
    WeatherApiStatusCode_t weatherErrorCode = getWeather( &s_weather );
    while ( weatherErrorCode != Success )
    {
        LogError( "Request for weather failed. Code: %d", weatherErrorCode );
        vTaskDelay( MSEC_TO_TICK( 10000 ) );
        weatherErrorCode = getWeather( &s_weather );
    }
}

static void requestDatetime()
{
    WeatherApiStatusCode_t datetimeErrorCode = getDatetime( &s_datetime );
    while ( datetimeErrorCode != Success )
    {
        LogError( "Request for datetime failed. Code: %d", datetimeErrorCode );
        vTaskDelay( MSEC_TO_TICK( 10000 ) );
        datetimeErrorCode = getDatetime( &s_datetime );
    }
}

void wifiTask( void *params )
{
    initSerialMWM();

    requestWeather();
    requestDatetime();

    logWeather( &s_weather );
    logDatetime( &s_datetime );
    setRTCDatetime( &s_datetime );

    for ( ;; )
    {
        logWlanStatus();

        s_datetime = getRTCDatetime();
        logDatetime( &s_datetime );

        vTaskDelay( MSEC_TO_TICK( 1000 ) );
    }
}

void swMainTimerCallback( TimerHandle_t xTimer )
{
    performWaterscreenAction( &s_context );
    logWaterscreenStatus( &s_context );
}
