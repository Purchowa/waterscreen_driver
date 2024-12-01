#ifndef CONVERTER_UTILS_H_
#define CONVERTER_UTILS_H_

#include "picture_types.h"
#include "neopixel_defines.h"
#include "container_types.h"
#include "color.h"


FloatSpan_t createFloatSpan( float *data, size_t size );

void binaryToArray( const pictureRow_t binaryRow, FloatSpan_t dst );
void interpolateNormalized( const FloatSpan_t yInput, FloatSpan_t yOutput );
void convertColorsRatioToRGBColors( const FloatSpan_t colorsRatio, ColorRGB_t *rgbColors, ColorRGB_t mainColor,
                                    ColorRGB_t secondaryColor, float secondaryColorFactor );


#endif /* CONVERTER_UTILS_H_ */
