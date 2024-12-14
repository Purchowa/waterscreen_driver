#ifndef WATERSCREEN_STATE_CONTEXT_H_
#define WATERSCREEN_STATE_CONTEXT_H_

#include "config/rtos_time_defines.h"
#include "picture_management/picture_types.h"

#include <stdbool.h>
#include <stdint.h>


typedef int32_t status_t;

struct WaterscreenContext;

typedef void ( *waterscreenStateFunction_t )( struct WaterscreenContext * );

typedef struct WaterscreenContext
{
    waterscreenStateFunction_t waterscreenStateHandler;
    waterscreenStateFunction_t previousWaterscreenStateHandler;
    const PictureInfo_t       *pictureInfo;
    int32_t                    valveOpenCounter;
    status_t                   currentStateStatus;
    delayMs_t                  currentStateDelay;
} WaterscreenContext_t;

extern WaterscreenContext_t g_context;

#endif /* WATERSCREEN_STATE_CONTEXT_H_ */
