#ifndef NEOPIXEL_CONVERTER_H_
#define NEOPIXEL_CONVERTER_H_

#include "color.h"
#include "picture_management/picture_types.h"


void convertPictureRowToGRBColors( const pictureRow_t pictureRow, colorGRB_t *grbColors, ColorRGB_t mainColor,
                                   ColorRGB_t secondaryColor, float secondaryColorFactor );

#endif /* NEOPIXEL_CONVERTER_H_ */
