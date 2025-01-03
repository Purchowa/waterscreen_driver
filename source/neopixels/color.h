#ifndef COLOR_H_
#define COLOR_H_

#include <stdint.h>

typedef struct
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} ColorRGB_t;

typedef uint32_t colorGRB_t;

colorGRB_t rgbToGrb( const ColorRGB_t color );

#endif /* COLOR_H_ */
