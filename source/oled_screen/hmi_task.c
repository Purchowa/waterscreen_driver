#include "hmi_task.h"
#include "oled_screen/oled_panel.h"

#include "gpio/button_control.h"

#include "waterscreen_config.h"
#include "waterscreen_state/waterscreen_state_context.h"
#include "waterscreen_state/waterscreen_states.h"
#include "waterscreen_state/standard_mode_state.h"
#include "waterscreen_state/waterscreen_state_context_handler.h"

#include "external_communication/wifi_task.h"

#include <FreeRTOS.h>
#include <task.h>


#define HMI_TASK_DELAY 32

void hmiTask( void *params )
{
    for ( ;; )
    {
        drawInfoPanel( &g_context, g_waterscreenConfig.mode, getLastHttpCode() );

        // USER
        if ( isS3ButtonPressed() )
        {
            forceChangeWaterscreenState( &g_context, demoModeState );
            g_waterscreenConfig.mode = Mode_Demo;
        }

        // WAKEUP
        if ( isS2ButtonPressed() )
        {
            forceChangeWaterscreenState( &g_context, standardModeState );
            g_waterscreenConfig.mode = Mode_Standard;
        }

        // ISP
        if ( isS1ButtonPressed() )
        {
            forceChangeWaterscreenState( &g_context, idleState );
            g_waterscreenConfig.mode = Mode_Service;
        }

        vTaskDelay( pdMS_TO_TICKS( HMI_TASK_DELAY ) );
    }
}
