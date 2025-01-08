#include "presentation_state_machine_task.h"

#include "standard_mode_state.h"
#include "waterscreen_config.h"
#include "waterscreen_state/waterscreen_state_context.h"
#include "waterscreen_state/waterscreen_state_context_handler.h"

#include "status_logging.h"

#include <FreeRTOS.h>
#include <task.h>


void presentationStateMachineTask( void *params )
{
    initStandardModeConfig( &g_waterscreenConfig.standardModeConfig );

    TickType_t lastWakeTime = xTaskGetTickCount();
    for ( ;; )
    {
        performWaterscreenAction( &g_context );
        logWaterscreenStatus( &g_context );

        vTaskDelayUntil( &lastWakeTime, pdMS_TO_TICKS( g_context.stateDelay ) );
    }
}
