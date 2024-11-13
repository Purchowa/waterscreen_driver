#ifndef WATERSCREEN_STATE_CONTEXT_H_
#define WATERSCREEN_STATE_CONTEXT_H_

#include "picture_management/picture_types.h"
#include "rtos_tasks/rtos_time_defines.h"

#include <stdbool.h>
#include <stdint.h>


typedef int32_t status_t;

struct WaterscreenContext;

typedef void ( *waterscreenStateFunction_t )( struct WaterscreenContext * );

typedef struct WaterscreenContext
{
    waterscreenStateFunction_t waterscreenStateHandler;
    waterscreenStateFunction_t previousWaterscreenStateHandler;
    const PictureDataSpan_t   *picture;
    int32_t                    valveOpenCounter;
    status_t                   currentStateStatus;
    delayMs_t                  currentStateDelay;
} WaterscreenContext_t;


void changeWaterscreenState( WaterscreenContext_t *context, waterscreenStateFunction_t newState );
void forceChangeWaterscreenState( WaterscreenContext_t *context, waterscreenStateFunction_t newState );
void performWaterscreenAction( WaterscreenContext_t *context );
void goBackToPreviousWaterscreenState( WaterscreenContext_t *context );

#endif /* WATERSCREEN_STATE_CONTEXT_H_ */
