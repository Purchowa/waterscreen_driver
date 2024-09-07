#ifndef PICTURES_H_
#define PICTURES_H_

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

const pictureDataView_t *getPictureView();
customPictureDataSpan_t *getCustomPictureSpan();

#endif /* PICTURES_H_ */
