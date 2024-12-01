#include "converter_utils.h"

#include <assert.h>

#define BITS_IN_BYTE  8
#define LAST_BIT_MASK 1


FloatSpan_t createFloatSpan( float *data, size_t size )
{
    const FloatSpan_t span = { .data = data, .size = size };

    return span;
}

void binaryToArray( const pictureRow_t binaryRow, FloatSpan_t dst )
{
    for ( int32_t i = dst.size - 1, bitIdx = 0; 0 <= i; --i, ++bitIdx )
    {
        const uint8_t bit = ( binaryRow >> (uint32_t)bitIdx ) & LAST_BIT_MASK;
        dst.data[i]       = bit;
    }
}

void interpolateNormalized( const FloatSpan_t yInput, FloatSpan_t yOutput )
{
    assert( 0 < yOutput.size && 0 < yInput.size );
    assert( yInput.size <= yOutput.size );

    if ( yInput.size == 1 )
    {
        for ( uint32_t i = 0; i < yOutput.size; ++i )
        {
            yOutput.data[i] = yInput.data[0];
        }
        return;
    }

    const int scale = ( yOutput.size - 1 ) / ( yInput.size - 1 );

    for ( uint32_t i = 0; i < yInput.size - 1; ++i )
    {
        for ( uint32_t scaleStep = 0; scaleStep < scale; ++scaleStep )
        {
            const float t = scaleStep / (float)scale;

            const float y0 = yInput.data[i];
            const float y1 = yInput.data[i + 1];

            yOutput.data[i * scale + scaleStep] = y0 * ( 1 - t ) + y1 * t;
        }
    }

    yOutput.data[yOutput.size - 1] = yInput.data[yInput.size - 1];
}

void convertColorsRatioToRGBColors( const FloatSpan_t colorsRatio, ColorRGB_t *rgbColors, ColorRGB_t mainColor,
                                    ColorRGB_t secondaryColor, float secondaryColorFactor )
{
    for ( uint32_t i = 0; i < colorsRatio.size; ++i )
    {
        const float mainRatio = colorsRatio.data[i];
        const float secRatio  = 1 - mainRatio;

        rgbColors[i].r = secRatio * secondaryColor.r * secondaryColorFactor + mainRatio * mainColor.r;
        rgbColors[i].g = secRatio * secondaryColor.g * secondaryColorFactor + mainRatio * mainColor.g;
        rgbColors[i].b = secRatio * secondaryColor.b * secondaryColorFactor + mainRatio * mainColor.b;
    }
}
