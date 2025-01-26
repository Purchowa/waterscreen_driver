#ifndef NEOPIXEL_PROVIDER_H_
#define NEOPIXEL_PROVIDER_H_

#include "color.h"
#include "picture_management/picture_types.h"

void dimNeopixels();
void lightUpNeopixels( const pictureRow_t pictureRow, ColorRGB_t mainColor, ColorRGB_t secondaryColor );
void lightUpNeopixelsWithColor( ColorRGB_t color );

#endif /* NEOPIXEL_PROVIDER_H_ */
