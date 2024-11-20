#include "picture_management/demo_mode_picture_logic.h"
#include "picture_management/picture_logic_utils.h"
#include "waterscreen_states.h"
#include "waterscreen_state_context.h"

#include "gpio/power_control.h"
#include "gpio/sensors_control.h"

#include "spi_transfer/spi_transfer.h"


void demoModeState( WaterscreenContext_t *context )
{
    context->picture          = getEachPicture();
    context->valveOpenCounter = getLastPictureIndex( context->picture );
    manageValvePower( OnDeviceState );
    changeWaterscreenState( context, presentationState );
}

void checkSensorsSubState( WaterscreenContext_t *context )
{
    const bool isWaterAlarmTriggered = shouldWaterAlarmTrigger();

    if ( isWaterAlarmTriggered )
    {
        manageValvePower( OffDeviceState );
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
        closeValvesSubState( context );
        goBackToPreviousWaterscreenState( context );
        context->currentStateDelay = BETWEEN_PICTURES_DELAY_MS;
    }
    else
    {
        const status_t status       = sendDataToValves( &context->picture->data[context->valveOpenCounter--] );
        context->currentStateStatus = status;
        context->currentStateDelay  = PRESENTING_DELAY_MS;
    }
}

void closeValvesSubState( WaterscreenContext_t *context )
{
    static const uint64_t closeValves = 0;

    const status_t status       = sendDataToValves( &closeValves );
    context->currentStateStatus = status;
}

void idleState( WaterscreenContext_t *context )
{
    checkSensorsSubState( context );

    context->currentStateDelay = SECOND_MS;
}

void lowWaterState( WaterscreenContext_t *context )
{
    manageWaterPump( OffDeviceState );
    if ( !shouldWaterAlarmTrigger() )
    {
        goBackToPreviousWaterscreenState( context );
        context->currentStateDelay = 5 * SECOND_MS;
    }
    else
    {
        context->currentStateDelay = SECOND_MS;
    }
}
