#ifndef PICTURE_TYPES_H_
#define PICTURE_TYPES_H_

#include "datetime/datetime_types.h"
#include "external_communication/weather_types.h"

#include <stdint.h>

typedef struct
{
    const uint8_t   size;
    const uint64_t *data;
} pictureDataView_t;

typedef struct
{
    const uint8_t capacity;
    uint8_t       size;
    uint64_t     *data;
} customPictureDataSpan_t;

#endif /* PICTURE_TYPES_H_ */
