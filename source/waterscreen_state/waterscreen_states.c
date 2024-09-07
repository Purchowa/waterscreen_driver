#include "waterscreen_states.h"
#include "waterscreen_state_context.h"
#include "pictures.h"
#include "gpio/power_control.h"
#include "gpio/sensors_control.h"
#include "spi_transfer/spi_transfer.h"
#include "datetime/datetime_types.h"
#include "datetime/rtc_provider.h"

#include <assert.h>

void standardModeState( WaterscreenContext_t *context )
{
    Datetime_t datetime;
    getRTCDatetime( &datetime );
}

static inline uint16_t lastElementIndex( const pictureDataView_t *picture )
{
    assert( picture );
    return ( picture->size - 1 );
}

void demoModeState( WaterscreenContext_t *context )
{
    context->picture          = getPicture();
    context->valveOpenCounter = lastElementIndex( context->picture );
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
        const status_t status       = sendDataToValves( &context->picture->data[context->valveOpenCounter--] );
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
