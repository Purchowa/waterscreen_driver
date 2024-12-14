#ifndef WATERSCREEN_CONFIG_H_
#define WATERSCREEN_CONFIG_H_

#include "waterscreen_state/waterscreen_state_context.h"
#include "picture_management/picture_data_constants.h"
#include "picture_management/picture_data.h"

#include <stdint.h>
#include <stdbool.h>


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
    PictureInfo_t *const customPicture;
} WaterscreenConfig_t;

extern WaterscreenConfig_t              g_waterscreenConfig;
extern const waterscreenStateFunction_t g_waterscreenModes[Mode_SIZE];

#endif /* WATERSCREEN_CONFIG_H_ */
