#include "neopixel_converter.h"
#include "neopixel_defines.h"
#include "converter_utils.h"
#include "color.h"

#include <assert.h>


void convertPictureRowToGRBColors( const pictureRow_t pictureRow, colorGRB_t *grbColors, ColorRGB_t mainColor,
                                   ColorRGB_t secondaryColor, float secondaryColorFactor )
{
    assert( PICTURE_WIDTH < NEOPIXEL_COUNT );

    static float bitArray[PICTURE_WIDTH];
    FloatSpan_t  bits = { .data = bitArray, .size = PICTURE_WIDTH };
    binaryToArray( pictureRow, bits );

    static float colorsRatioArray[NEOPIXEL_COUNT];
    FloatSpan_t  colorsRatio = { .data = colorsRatioArray, .size = NEOPIXEL_COUNT };
    interpolateNormalized( bits, colorsRatio );

    static ColorRGB_t rgbColors[NEOPIXEL_COUNT];
    convertColorsRatioToRGBColors( colorsRatio, rgbColors, mainColor, secondaryColor, secondaryColorFactor );

    for ( uint32_t i = 0; i < NEOPIXEL_COUNT; ++i )
    {
        grbColors[i] = rgbToGrb( rgbColors[i] );
    }
}
