#ifndef WATERSCREEN_STATE_CONTEXT_H_
#define WATERSCREEN_STATE_CONTEXT_H_

#include "pictures.h"

#include <stdbool.h>
#include <stdint.h>

typedef int32_t status_t;

struct WaterscreenContext;

typedef void ( *waterscreenStateFunction_t )( struct WaterscreenContext * );

typedef struct WaterscreenContext
{
    waterscreenStateFunction_t waterscreenStateHandler;
    waterscreenStateFunction_t previousWaterscreenStateHandler;
    const pictureData_t       *picture;
    const uint8_t              demoLoopCount; // Delete in future
    int32_t                    valveOpenCounter;
    status_t                   currentStateStatus;
} WaterscreenContext_t;


void changeWaterscreenState( WaterscreenContext_t *context, waterscreenStateFunction_t newState );
void performWaterscreenAction( WaterscreenContext_t *context );
void goBackToPreviousWaterscreenState( WaterscreenContext_t *context );

#endif /* WATERSCREEN_STATE_CONTEXT_H_ */
