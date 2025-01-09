#ifndef RECEIVER_DATA_TYPES_H_
#define RECEIVER_DATA_TYPES_H_

#include <stdint.h>

typedef enum
{
    Text,
    Configuration,
    CustomPicture,
    RTModeActive,
    WiFiCredentials
} DataType_t;

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

typedef uint8_t typeInfo_t;

typedef uint8_t textSize_t;
typedef uint8_t rtModeActive_t;
typedef uint8_t wifiCredentialsSize_t;

#endif /* RECEIVER_DATA_TYPES_H_ */
