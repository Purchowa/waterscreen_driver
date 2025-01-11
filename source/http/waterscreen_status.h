#ifndef WATERSCREEN_STATUS_H_
#define WATERSCREEN_STATUS_H_

#include "waterscreen_state/waterscreen_state_context.h"
#include "config/waterscreen_config.h"

#include <stdbool.h>


typedef enum
{
    FluidLevel_Optimal,
    FluidLevel_Low
} FluidLevel_t;

typedef struct
{
    WaterscreenMode_t mode;
    FluidLevel_t      fluidLevel;
    bool              isPresenting;
} WaterscreenStatus_t;

WaterscreenStatus_t generateWaterscreenStatus( const WaterscreenMode_t     currentMode,
                                               const WaterscreenContext_t *context );

#endif /* WATERSCREEN_STATUS_H_ */
