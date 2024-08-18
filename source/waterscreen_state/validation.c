/*
 * validation.c
 *
 *  Created on: 20 maj 2024
 *      Author: purch
 */
#include "validation.h"

#include <fsl_common.h>
#include <stdint.h>

#include "config/logging_config.h"
#include "waterscreen_states.h"

#define STATE_COUNT 4

typedef struct
{
    waterscreenStateFunction_t key;
    const char                *value;
} pair_t;

static const char *getCurrentStateName( WaterscreenContext_t *context )
{
    static const pair_t statesMap[STATE_COUNT] = { { .key = demoModeState, .value = "demo mode" },
                                                   { presentationState, "presentation" },
                                                   { idleState, "idle" },
                                                   { lowWaterState, "low water" } };
    static const char  *unknwonState           = "unknown";

    for ( uint8_t i = 0; i < STATE_COUNT; ++i )
    {
        if ( context->waterscreenStateHandler == statesMap[i].key )
            return statesMap[i].value;
    }

    return unknwonState;
}

void logWaterscreenStatus( WaterscreenContext_t *context )
{
    if ( context->currentStateStatus == kStatus_Fail )
        LogError( "SPI transfer failed" );

    static waterscreenStateFunction_t previousState = NULL;
    if ( context->waterscreenStateHandler != previousState )
    {
        LogInfo( "Current state: %s", getCurrentStateName( context ) );
        previousState = context->waterscreenStateHandler;
    }
}
