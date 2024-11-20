#ifndef CONVERTER_UTILS_H_
#define CONVERTER_UTILS_H_

#include "picture_types.h"
#include "neopixel_defines.h"


void binaryToArray( const pictureRow_t binaryRow, byte_t *dstArray, const size_t bitArraySize );

#endif /* CONVERTER_UTILS_H_ */
