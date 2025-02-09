#ifndef PICTURE_DATA_CONSTANTS_H_
#define PICTURE_DATA_CONSTANTS_H_

#ifndef PICTURE_WIDTH
#define PICTURE_WIDTH 64 // number bits in a picture
#endif

// Constants describing how many pictures to print.
#define ALL_PICTURE_COUNT                  28 // Used in demo
#define STANDARD_MODE_STATIC_PICTURE_COUNT 13
// ---

#define STANDARD_MODE_PICTURE_GETTER_COUNT 6

#define MAX_CUSTOM_PICTURE_HEIGHT 800

#define NUMBER_OF_ROWS_IN_PICTURE_CHARACTER 24

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
