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
    PRINTF( "Initializing...\r\n" );
    int ret = mwm_init();
    if ( ret < 0 )
    {
        PRINTF( "Could not initialize Serial MWM, error: %d\r\n", ret );
        while ( 1 )
            ;
    }
    ret = wlan_get_state();
    if ( ret == MWM_INITIALIZED )
    {
        PRINTF( "Starting WLAN...\r\n" );
        ret = mwm_wlan_start();
        if ( ret < 0 )
        {
            PRINTF( "Could not start WLAN subsystem, error: %d\r\n", ret );
            while ( 1 )
                ;
        }
    }
    wlan_config( AP_SSID, AP_PASSPHRASE, AP_SECURITY_MODE );
    while ( wlan_get_state() != MWM_CONNECTED )
        ;
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
        // wlan_state();
        vTaskDelay( MSEC_TO_TICK( 1000 ) );
    }
}

void swMainTimerCallback( TimerHandle_t xTimer )
{
    performWaterscreenAction( &s_context );
    logWaterscreenStatus( &s_context );
}
