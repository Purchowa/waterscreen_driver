#ifndef PICTURE_TYPES_H_
#define PICTURE_TYPES_H_

#include "datetime/datetime_types.h"
#include "picture_data_constants.h"

#include <stdint.h>

#define RESIZABLE_PICTURE_VIEW_ARRAY_CAPACITY 8

typedef uint64_t pictureRow_t;
typedef uint16_t pictureCharacter_t;

typedef struct
{
    uint8_t       size;
    pictureRow_t *data;
} PictureDataSpan_t;

typedef struct
{
    const uint8_t capacity;
    uint8_t       size;
    pictureRow_t  data[MAX_CUSTOM_PICTURE_LENGTH];
} ResizableCustomPicture_t;

/**
 * @brief Array of `PictureSpan` type that its size can vary from 0 to `RESIZABLE_PICTURE_VIEW_ARRAY_CAPACITY`
 */
typedef struct
{
    const uint8_t     size;
    PictureDataSpan_t data[RESIZABLE_PICTURE_VIEW_ARRAY_CAPACITY];
} ResizablePictureSpanArray_t;

typedef struct
{
    const ShortDate_t seasonDateStart;
    PictureDataSpan_t pictureSpan;
} SeasonalPicture_t;

typedef struct
{
    const ShortDate_t from;
    const ShortDate_t to;
} HolidaysRange_t;

typedef struct
{
    const HolidaysRange_t       range;
    ResizablePictureSpanArray_t pictureSpanArray;
} HolidaysPictureArray_t;

#endif /* PICTURE_TYPES_H_ */
