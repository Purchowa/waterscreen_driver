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
    const float step = ( yInput.size - 1 ) / (float)( yOutput.size - 1 );

    for ( size_t i = 0; i < yOutput.size; ++i )
    {
        const float  pos     = i * step;
        const size_t lowIdx  = (size_t)pos;
        size_t       highIdx = lowIdx + 1;

        if ( yInput.size <= highIdx )
            highIdx = yInput.size - 1;

        const float t   = pos - lowIdx;
        yOutput.data[i] = yInput.data[lowIdx] * ( 1.f - t ) + yInput.data[highIdx] * t;
    }
}

void convertColorsRatioToRGBColors( const FloatSpan_t colorsRatio, ColorRGB_t *rgbColors, ColorRGB_t mainColor,
                                    ColorRGB_t secondaryColor, float mainColorFactor, float secondaryColorFactor )
{
    for ( uint32_t i = 0; i < colorsRatio.size; ++i )
    {
        const float mainRatio = colorsRatio.data[i];
        const float secRatio  = 1 - mainRatio;

        rgbColors[i].r = secRatio * secondaryColor.r * secondaryColorFactor + mainRatio * mainColor.r * mainColorFactor;
        rgbColors[i].g = secRatio * secondaryColor.g * secondaryColorFactor + mainRatio * mainColor.g * mainColorFactor;
        rgbColors[i].b = secRatio * secondaryColor.b * secondaryColorFactor + mainRatio * mainColor.b * mainColorFactor;
    }
}

ColorRGB_t adjustColorBrightness( ColorRGB_t color, float factor )
{
    color.r *= factor;
    color.g *= factor;
    color.b *= factor;

    return color;
}

void copyReverse( colorGRB_t *dest, const colorGRB_t *src, size_t n )
{
    for ( size_t i = 0; i < n; ++i )
        dest[i] = src[n - i - 1];
}
