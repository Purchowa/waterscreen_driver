#ifndef PICTURES_H_
#define PICTURES_H_

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

const pictureDataView_t *getEachPictureView();
const pictureDataView_t *getOccasionalPictureView( const Date_t *, const WeatherCondition_t );

customPictureDataSpan_t *getCustomPictureSpan();

#endif /* PICTURES_H_ */
