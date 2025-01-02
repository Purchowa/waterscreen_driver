#include "waterscreen_config.h"


void setWaterscreenMode( ModeInfo_t *modeInfo, WaterscreenMode_t mode )
{
    modeInfo->previous = modeInfo->current;
    modeInfo->current  = mode;
}
