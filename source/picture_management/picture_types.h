#ifndef PICTURE_TYPES_H_
#define PICTURE_TYPES_H_

#include "datetime/datetime_types.h"
#include "picture_data_constants.h"
#include "neopixels/color.h"

#include <stdint.h>

#define RESIZABLE_PICTURES_CAPACITY 8

typedef uint32_t pictureSize_t;
typedef uint64_t pictureRow_t;
typedef uint16_t pictureCharacter_t;

typedef struct
{
    pictureSize_t size;
    pictureRow_t *data;
} PictureDataSpan_t;

typedef struct
{
    ColorRGB_t main;
    ColorRGB_t secondary;
} PictureColors_t;

typedef struct
{
    PictureDataSpan_t picture;
    PictureColors_t   colors;
} PictureInfo_t;

/**
 * @brief Resizable container that its size can vary, but has constant capacity
 */
typedef struct
{
    const pictureSize_t size;
    PictureInfo_t      *data[RESIZABLE_PICTURES_CAPACITY];
} ResizablePictureInfoArray_t;

typedef struct
{
    const ShortDate_t seasonDateStart;
    PictureInfo_t    *picture;
} SeasonalPicture_t;

typedef struct
{
    const ShortDate_t from;
    const ShortDate_t to;
} HolidaysRange_t;

typedef struct
{
    const HolidaysRange_t       range;
    ResizablePictureInfoArray_t pictures;
} HolidaysPictureArray_t;

#endif /* PICTURE_TYPES_H_ */
