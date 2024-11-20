#ifndef NEOPIXEL_CONVERTER_H_
#define NEOPIXEL_CONVERTER_H_

#include "neopixel_defines.h"
#include "color.h"

#include "picture_types.h"


void convertPictureRowToGRBColors( colorGRB_t *colorsBuff, const pictureRow_t *picture );

#endif /* NEOPIXEL_CONVERTER_H_ */
