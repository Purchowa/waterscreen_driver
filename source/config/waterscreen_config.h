#ifndef WATERSCREEN_CONFIG_H_
#define WATERSCREEN_CONFIG_H_

#include <stdint.h>
#include <stdbool.h>

#define MAX_CUSTOM_PICTURE_LENGTH 64


typedef enum
{
    Standard,
    Demo,
    Service,
    SIZE
} WaterscreenMode_t;

typedef struct
{
    bool     isWorkingDuringWeekends;
    uint16_t workTimeInStandardMode;
    uint16_t idleTimeInStandardMode;
} StandardModeConfig_t;

typedef struct
{
    WaterscreenMode_t    mode;
    StandardModeConfig_t standardModeConfig;
    uint8_t              customPictureSize;
    uint64_t             customPicture[MAX_CUSTOM_PICTURE_LENGTH];
} WaterscreenConfig_t;

#endif /* WATERSCREEN_CONFIG_H_ */
