#ifndef PICTURE_TYPES_H_
#define PICTURE_TYPES_H_

#include "datetime/datetime_types.h"
#include "external_communication/weather_types.h"

#include <stdint.h>

typedef uint64_t pictureRow_t;
typedef uint16_t pictureCharacter_t;

typedef struct
{
    const uint8_t       size;
    const pictureRow_t *data;
} PictureDataView_t;

typedef struct
{
    const uint8_t capacity;
    uint8_t       size;
    pictureRow_t *data;
} CustomPictureDataSpan_t;

#define PICTURE_VIEW_SPAN_CAPACITY 16
typedef struct
{
    const uint8_t           size;
    const PictureDataView_t data[PICTURE_VIEW_SPAN_CAPACITY];
} PictureViewSpan_t;

typedef struct
{
    const ShortDate_t       seasonDateStart;
    const PictureDataView_t pictureView;
} SeasonsInfo_t;

typedef struct
{
    const ShortDate_t dateFrom;
    const ShortDate_t dateTo;
} SpecialDaysInterval_t;

typedef struct
{
    SpecialDaysInterval_t interval;

} SpecialDaysInfo_t;

#endif /* PICTURE_TYPES_H_ */
