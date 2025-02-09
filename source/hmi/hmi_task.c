#include "hmi_task.h"
#include "lcd_panel.h"

#include "gpio/button_control.h"

#include "waterscreen_config.h"
#include "waterscreen_state/waterscreen_state_context.h"
#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/standard_mode_state.h"
#include "waterscreen_state/waterscreen_state_context_handler.h"
#include "waterscreen_state/state_utils.h"

#include "http/wifi_task.h"
#include "logging.h"

#include <FreeRTOS.h>
#include <task.h>


#define TICKS_TO_MS( ticks ) ( ( ticks ) * 1000 / configTICK_RATE_HZ )
#define HMI_TASK_DELAY       32

#define TRANSFER_TO_IDLE_HOLD_TIME_MS           1000
#define TRANSFER_TO_SELECTION_MODE_HOLD_TIME_MS 1500

typedef void ( *menuState_t )( void );

void idleMenuState();
void modeSelectionMenuState();

static menuState_t s_menuState = idleMenuState;

void idleMenuState()
{
    static TickType_t pressStart  = 0;
    static bool       changeState = false;

    if ( isS1ButtonPressed() && pressStart == 0 ) // start press
    {
        pressStart = xTaskGetTickCount();
    }
    else if ( isS1ButtonPressed() && pressStart != 0 ) // remain press
    {
        const int32_t pressTime = TICKS_TO_MS( xTaskGetTickCount() - pressStart );

        if ( TRANSFER_TO_SELECTION_MODE_HOLD_TIME_MS <= pressTime )
        {
            pressStart  = 0;
            changeState = true;
        }
    }
    else if ( !isS1ButtonPressed() && pressStart != 0 ) // release press
    {
        pressStart = 0;
        if ( changeState )
        {
            s_menuState = modeSelectionMenuState;
            changeState = false;
            return;
        }
    }

    drawInfoPanel( &g_context, g_waterscreenConfig.mode.current, getLastHttpCode(), changeState );
}

void modeSelectionMenuState()
{
    static TickType_t        pressStart = 0;
    static WaterscreenMode_t mode       = 0;

    if ( isS1ButtonPressed() && pressStart == 0 )
    {
        pressStart = xTaskGetTickCount();
    }
    else if ( isS1ButtonPressed() && pressStart != 0 )
    {
        const int32_t pressTime = TICKS_TO_MS( xTaskGetTickCount() - pressStart );
        if ( TRANSFER_TO_IDLE_HOLD_TIME_MS <= pressTime )
        {
            LogDebug( "Mode changed to %s", getModeName( mode ) );
            pressStart = 0;

            setWaterscreenMode( &g_waterscreenConfig.mode, mode );
            forceChangeWaterscreenState( &g_context,
                                         g_waterscreenConfigAvailableModes[g_waterscreenConfig.mode.current] );

            s_menuState = idleMenuState;
        }
    }
    else if ( !isS1ButtonPressed() && pressStart != 0 )
    {
        pressStart = 0;
        mode       = ( mode + 1 ) % CONFIG_AVAILABLE_MODE_COUNT;
        LogDebug( "Mode selected: %s", getModeName( mode ) );
    }
    drawInfoPanel( &g_context, mode, getLastHttpCode(), true );
}

typedef bool ( *isButtonPressedFun_t )();
static bool isButtonHeld( isButtonPressedFun_t isPressed )
{
    static TickType_t pressStart = 0;

    if ( isPressed() && pressStart == 0 )
    {
        pressStart = xTaskGetTickCount();
    }
    else if ( isPressed() && pressStart != 0 )
    {
        const int32_t pressTime = TICKS_TO_MS( xTaskGetTickCount() - pressStart );
        return TRANSFER_TO_SELECTION_MODE_HOLD_TIME_MS <= pressTime;
    }

    pressStart = 0;
    return false;
}

static void quickMenu()
{
    static bool wasS2Held = false;

    if ( isButtonHeld( isS2ButtonPressed ) )
    {
        wasS2Held = true;
        LogInfo( "Held" );
    }
    else if ( wasS2Held )
    {
        const WaterscreenMode_t mode = g_waterscreenConfig.mode.current != Mode_Demo ? Mode_Demo : Mode_Standard;
        setWaterscreenMode( &g_waterscreenConfig.mode, mode );
        forceChangeWaterscreenState( &g_context, g_waterscreenConfigAvailableModes[mode] );

        wasS2Held = false;
    }
}

void hmiTask( void *params )
{
    bool wasS2Pressed = false;
    for ( ;; )
    {
        s_menuState();
        // quickMenu();
        if ( isS2ButtonPressed() )
        {
            wasS2Pressed = true;
        }
        else if ( wasS2Pressed )
        {
            const WaterscreenMode_t mode = g_waterscreenConfig.mode.current != Mode_Demo ? Mode_Demo : Mode_Standard;
            setWaterscreenMode( &g_waterscreenConfig.mode, mode );
            forceChangeWaterscreenState( &g_context, g_waterscreenConfigAvailableModes[mode] );

            wasS2Pressed = false;
        }

        vTaskDelay( pdMS_TO_TICKS( HMI_TASK_DELAY ) );
    }
}
