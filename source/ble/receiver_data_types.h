#ifndef RECEIVER_DATA_TYPES_H_
#define RECEIVER_DATA_TYPES_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum
{
    LogsActive,
    Configuration,
    CustomPicture,
    RTModeActive,
    WiFiCredentials
} DataType_t;

typedef uint8_t typeInfo_t;

typedef uint8_t textSize_t;
typedef uint8_t rtModeActive_t;
typedef uint8_t wifiCredentialsSize_t;

#pragma pack( push, 1 )
typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} SerializedColorRGB_t;
#pragma pack( pop )

#pragma pack( push, 1 )
typedef struct
{
    SerializedColorRGB_t main;
    SerializedColorRGB_t secondary;
} SerializedPictureColors_t;
#pragma pack( pop )

#pragma pack( push, 1 )
typedef struct
{
    uint8_t  waterscreenMode;
    bool     isWorkingDuringWeekends;
    uint16_t workTimeInStandardMode;
    uint16_t idleTimeInStandardMode;
    uint8_t  workFrom;
    uint8_t  workTo;
} SerializedConfiguration_t;
#pragma pack( pop )

#endif /* RECEIVER_DATA_TYPES_H_ */
