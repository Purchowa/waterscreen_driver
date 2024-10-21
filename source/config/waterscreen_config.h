#ifndef WATERSCREEN_CONFIG_H_
#define WATERSCREEN_CONFIG_H_

#include "waterscreen_state/waterscreen_state_context.h"

#include <stdint.h>
#include <stdbool.h>

#define MAX_CUSTOM_PICTURE_LENGTH 64


typedef enum
{
    Mode_Standard,
    Mode_Demo,
    Mode_Service,
    Mode_SIZE
} WaterscreenMode_t;

typedef struct
{
    uint8_t from;
    uint8_t to;
} Range_t;

typedef struct
{
    bool     isWorkingDuringWeekends;
    uint16_t workTimeInStandardMode;
    uint16_t idleTimeInStandardMode;
    Range_t  workRange;
} StandardModeConfig_t;

typedef struct
{
    WaterscreenMode_t    mode;
    StandardModeConfig_t standardModeConfig;
    uint8_t              customPictureSize;
    uint64_t             customPicture[MAX_CUSTOM_PICTURE_LENGTH];
} WaterscreenConfig_t;

extern const waterscreenStateFunction_t g_waterscreenModes[Mode_SIZE];

#endif /* WATERSCREEN_CONFIG_H_ */
