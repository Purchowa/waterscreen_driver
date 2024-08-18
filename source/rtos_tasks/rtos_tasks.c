/*
 * hmi_task.c
 *
 *  Created on: 2 cze 2024
 *      Author: purch
 */

#include "rtos_tasks.h"
#include "button_control.h"
#include "waterscreen_states.h"
#include "waterscreen_state_context.h"
#include "validation.h"
#include "WiFiCfg.h"
#include "logging_config.h"

#include "wlan/wlan_mwm.h"
#include <fsl_common.h>
#include <serial_mwm.h>

static WaterscreenContext_t s_context = { .waterscreenStateHandler         = idleState,
                                          .previousWaterscreenStateHandler = idleState,
                                          .picture                         = NULL,
                                          .demoLoopCount                   = 4,
                                          .valveOpenCounter                = 0,
                                          .currentStateStatus              = kStatus_Success };

static void initSerialMWM()
{
    LogInfo( "Initializing serial MWM" );
    int ret = mwm_init();
    if ( ret < 0 )
    {
        LogError( "Could not initialize Serial MWM, error: %d", ret );
        while ( 1 )
            ;
    }
    ret = wlan_get_state();
    if ( ret == MWM_INITIALIZED )
    {
        LogInfo( "Starting WLAN" );
        ret = mwm_wlan_start();
        if ( ret < 0 )
        {
            LogError( "Could not start WLAN subsystem, error: %d", ret );
            while ( 1 )
                ;
        }
    }
    wlan_config( AP_SSID, AP_PASSPHRASE, AP_SECURITY_MODE );

    int currentWlanState = wlan_get_state();
    while ( currentWlanState != MWM_CONNECTED )
    {
        if ( wlan_get_state() != currentWlanState )
            LogDebug( "WLAN didn't connect. Current state: %d", currentWlanState );

        currentWlanState = wlan_get_state();
    }

    wlan_state();
}

void hmiTask( void *params )
{
    for ( ;; )
    {
        if ( isS3ButtonPressed() && s_context.waterscreenStateHandler == idleState )
        {
            changeWaterscreenState( &s_context, demoModeState );
        }

        if ( isS2ButtonPressed() )
        {
            closeValvesSubState( &s_context );
            changeWaterscreenState( &s_context, idleState );
        }
    }
    vTaskSuspend( NULL ); // Basically kill task.
}

void wifiTask( void *params )
{
    initSerialMWM();

    for ( ;; )
    {
        logWlanStatus();
        vTaskDelay( MSEC_TO_TICK( 1000 ) );
    }
}

void swMainTimerCallback( TimerHandle_t xTimer )
{
    performWaterscreenAction( &s_context );
    logWaterscreenStatus( &s_context );
}
