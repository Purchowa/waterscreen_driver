#include <picture_management/demo_mode_picture_logic.h>
#include <picture_management/picture_logic_utils.h>
#include "waterscreen_states.h"
#include "waterscreen_state_context.h"

#include "gpio/power_control.h"
#include "gpio/sensors_control.h"

#include "spi_transfer/spi_transfer.h"


void demoModeState( WaterscreenContext_t *context )
{
    context->pictureView      = getEachPictureView();
    context->valveOpenCounter = getLastPictureIndex( context->pictureView );
    manageValvePower( OnDeviceState );
    changeWaterscreenState( context, presentationState );
}

void checkSensorsSubState( WaterscreenContext_t *context )
{
    const bool isWaterAlarmTriggered = shouldWaterAlaramTrigger();

    if ( isWaterAlarmTriggered )
    {
        // Post waterAlaram to API.
        closeValvesSubState( context );
        changeWaterscreenState( context, lowWaterState );
    }

    if ( shouldWaterPumpTrigger() && !isWaterAlarmTriggered )
    {
        manageWaterPump( OnDeviceState );
    }
    else
    {
        manageWaterPump( OffDeviceState );
    }
}

void presentationState( WaterscreenContext_t *context )
{
    checkSensorsSubState( context );

    if ( context->valveOpenCounter < 0 )
    {
        goBackToPreviousWaterscreenState( context );
    }
    else
    {
        const status_t status       = sendDataToValves( &context->pictureView->data[context->valveOpenCounter--] );
        context->currentStateStatus = status;
    }
}

void closeValvesSubState( WaterscreenContext_t *context )
{
    static const uint64_t closeValves = 0;

    const status_t status       = sendDataToValves( &closeValves );
    context->currentStateStatus = status;

    manageValvePower( OffDeviceState );
}

void idleState( WaterscreenContext_t *context )
{
    // Check for days, weeks, post to API, even the timer might be here lower.
    checkSensorsSubState( context );
}

void lowWaterState( WaterscreenContext_t *context )
{
    manageWaterPump( OffDeviceState );
    if ( !shouldWaterAlaramTrigger() )
    {
        changeWaterscreenState( context, idleState );
    }
}
