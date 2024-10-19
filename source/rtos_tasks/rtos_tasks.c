#include "rtos_tasks.h"

#include "button_control.h"
#include "waterscreen_states.h"
#include "waterscreen_state_context.h"
#include "standard_mode_state.h"
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
                                          .currentStateStatus              = kStatus_Success,
                                          .currentStateDelay               = SECOND_MS };

static Weather_t s_weather;

static Datetime_t s_datetime;


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
    WeatherApiStatusCode_t weatherErrorCode = getWeather( &s_weather );
    while ( weatherErrorCode != Success )
    {
        LogError( "Request for weather failed. Code: %d", weatherErrorCode );
        vTaskDelay( pdMS_TO_TICKS( 10 * SECOND_MS ) );
        weatherErrorCode = getWeather( &s_weather );
    }
}

static void requestDatetime()
{
    WeatherApiStatusCode_t datetimeErrorCode = getDatetime( &s_datetime );
    while ( datetimeErrorCode != Success )
    {
        LogError( "Request for datetime failed. Code: %d", datetimeErrorCode );
        vTaskDelay( pdMS_TO_TICKS( 10 * SECOND_MS ) );
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

    forceChangeWaterscreenState( &s_context, standardModeState );

    for ( ;; )
    {
        logWlanStatus();

        vTaskDelay( pdMS_TO_TICKS( SECOND_MS ) );
    }
}

void waterscreenActionTask( void *params )
{
    TickType_t lastWakeTime = xTaskGetTickCount();

    for ( ;; )
    {
        performWaterscreenAction( &s_context );
        logWaterscreenStatus( &s_context );

        vTaskDelayUntil( &lastWakeTime, pdMS_TO_TICKS( s_context.currentStateDelay ) );
    }
}
