#include "waterscreen_states.h"
#include "waterscreen_state_context_handler.h"
#include "picture_management/demo_mode_picture_logic.h"
#include "picture_management/picture_logic_utils.h"

#include "gpio/power_control.h"
#include "gpio/sensors_control.h"

#include "spi_transfer/spi_transfer.h"
#include "neopixels/neopixel_provider.h"
#include "neopixels/converter_utils.h"
#include "neopixels/neopixel_defines.h"

#include <string.h>

static void closeValvesSubState( WaterscreenContext_t *context )
{
    const status_t status = sendDataToValves( 0 );
    context->stateStatus  = status;
}

void demoModeState( WaterscreenContext_t *context )
{
    context->pictureInfo      = getEachPicture();
    context->valveOpenCounter = getLastPictureIndex( &context->pictureInfo->picture );
    manageValvePower( OnDeviceState );
    dimNeopixels();
    changeWaterscreenState( context, presentationState );
    context->stateDelay = PRESENTING_DELAY_MS;
}

void checkSensorsSubState( WaterscreenContext_t *context )
{
    const bool isWaterAlarmTriggered = shouldWaterAlarmTrigger();

    if ( isWaterAlarmTriggered )
    {
        shutdownValves( context );
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

static pictureRow_t s_neopixelRowData = 0;
void                presentationState( WaterscreenContext_t *context )
{
    checkSensorsSubState( context );

    if ( context->valveOpenCounter < 0 )
    {
        const PictureColors_t *colors = &context->pictureInfo->colors;
        if ( context->pictureInfo->enableRowBitSum )
            lightUpNeopixels( s_neopixelRowData, colors->main, colors->secondary );
        else
            lightUpNeopixelsWithColor(
                adjustColorBrightness( context->pictureInfo->colors.secondary, SECONDARY_COLOR_FACTOR ) );

        s_neopixelRowData = 0;
        closeValvesSubState( context );
        goBackToPreviousWaterscreenState( context );
        context->stateDelay = BETWEEN_PICTURES_DELAY_MS;
    }
    else
    {
        const PictureDataSpan_t *picture       = &context->pictureInfo->picture;
        const PictureColors_t   *colors        = &context->pictureInfo->colors;
        const pictureRow_t       valvesRowData = picture->data[context->valveOpenCounter];

        if ( context->pictureInfo->enableRowBitSum )
            s_neopixelRowData |= valvesRowData;
        else
            s_neopixelRowData = valvesRowData;

        lightUpNeopixels( s_neopixelRowData, colors->main, colors->secondary );

        const status_t status = sendDataToValves( valvesRowData );

        --context->valveOpenCounter;
        context->stateStatus = status;
        context->stateDelay  = PRESENTING_DELAY_MS;
    }
}

void idleState( WaterscreenContext_t *context )
{
    checkSensorsSubState( context );
    dimNeopixels();

    context->stateDelay = BETWEEN_PICTURES_DELAY_MS;
}

void lowWaterState( WaterscreenContext_t *context )
{
    manageWaterPump( OffDeviceState );
    dimNeopixels();
    if ( !shouldWaterAlarmTrigger() )
    {
        goBackToPreviousWaterscreenState( context );
        manageValvePower( OnDeviceState );
        context->stateDelay = 5 * SECOND_MS;
    }
    else
    {
        context->stateDelay = SECOND_MS;
    }
}

void serviceModeState( WaterscreenContext_t *context )
{
    manageWaterPump( OffDeviceState );
    manageValvePower( OnDeviceState );
    dimNeopixels();
    const status_t status = sendDataToValves( UINT64_MAX );
    context->stateStatus  = status;
}

void shutdownValves( WaterscreenContext_t *context )
{
    closeValvesSubState( context );
    manageValvePower( OffDeviceState );
}
