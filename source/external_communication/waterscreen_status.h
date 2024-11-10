#ifndef WATERSCREEN_STATUS_H_
#define WATERSCREEN_STATUS_H_

#include "waterscreen_config.h"

#include <stdbool.h>


typedef enum
{
    FluidLevel_Optimal,
    FuildLevel_Low
} FluidLevel_t;

typedef struct
{
    WaterscreenMode_t mode;
    FluidLevel_t      fluidLevel;
    bool              isPresenting;
} WaterscreenStatus_t;

#endif /* WATERSCREEN_STATUS_H_ */
