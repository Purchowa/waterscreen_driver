#ifndef PICTURE_DATA_CONSTANTS_H_
#define PICTURE_DATA_CONSTANTS_H_

#define ALL_PICTURE_COUNT 22

#define STANDARD_MODE_STATIC_PICTURE_COUNT 7
#define STANDARD_MODE_PICTURE_GETTER_COUNT 7

#define MAX_CUSTOM_PICTURE_LENGTH 128

#define CHARACTER_TO_PICTURE_ROW_COUNT 24

#define SEASONS_COUNT  4
#define HOLIDAYS_COUNT 6

typedef enum
{
    ZeroIdx,
    OneIdx,
    TwoIdx,
    ThreeIdx,
    FourIdx,
    FiveIdx,
    SixIdx,
    SevenIdx,
    EightIdx,
    NineIdx,
    ColonIdx,
    CHARACTER_TO_PICTURE_MAP_SIZE
} CharacterToPictureMapIndex_t;


#endif /* PICTURE_DATA_CONSTANTS_H_ */
