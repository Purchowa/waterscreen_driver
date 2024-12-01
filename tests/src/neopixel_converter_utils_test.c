#include <assert.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "converter_utils.h"
#include <stdbool.h>

#define BIT_ARRAY_SIZE 64

void givenBinaryNumber_binaryToArray_returnAnArrayOfBits()
{
    float expectedBitArray[BIT_ARRAY_SIZE] = { 0 };
    expectedBitArray[63]                   = 1;
    expectedBitArray[62]                   = 0;
    expectedBitArray[61]                   = 1;

    const pictureRow_t binNum                   = 0b101;
    float              bitArray[BIT_ARRAY_SIZE] = {};

    binaryToArray( binNum, createFloatSpan( bitArray, BIT_ARRAY_SIZE ) );

    assert_memory_equal( expectedBitArray, bitArray, BIT_ARRAY_SIZE );
}

static void assertFloatContainersEqual( const FloatSpan_t lhs, const FloatSpan_t rhs, const float eps )
{
    assert_int_equal( lhs.size, rhs.size );

    for ( size_t i = 0; i < lhs.size; ++i )
    {
        assert_float_equal( lhs.data[i], rhs.data[i], eps );
    }
}

void givenTwoIncreasingSamples_interpolateNormalized_interpolate()
{
    const float yInput[]         = { 0, 1 };
    const float expectedOutput[] = { 0, 1 / 3.f, 2 / 3.f, 1 };

    static const size_t yOutSize       = 4;
    float               yOut[yOutSize] = {};

    interpolateNormalized( createFloatSpan( yInput, sizeof( yInput ) / sizeof( *yInput ) ),
                           createFloatSpan( yOut, yOutSize ) );

    assert_memory_equal( expectedOutput, yOut, yOutSize * sizeof( *expectedOutput ) );
}

void givenTwoDecreasingSamples_interpolateNormalized_interpolate()
{
    const float yInput[]         = { 1, 0 };
    const float expectedOutput[] = { 1, 2 / 3.f, 1 / 3.f, 0 };

    static const size_t yOutSize       = 4;
    float               yOut[yOutSize] = {};

    interpolateNormalized( createFloatSpan( yInput, sizeof( yInput ) / sizeof( *yInput ) ),
                           createFloatSpan( yOut, yOutSize ) );

    assertFloatContainersEqual( createFloatSpan( expectedOutput, sizeof( expectedOutput ) / sizeof( *expectedOutput ) ),
                                createFloatSpan( yOut, yOutSize ), 0.001f );
}

void givenTwoEqualSamples_interpolateNormalized_repeatInputByScale()
{
    const float yInput[]         = { 1, 1 };
    const float expectedOutput[] = { 1, 1, 1, 1 };

    static const size_t yOutSize       = 4;
    float               yOut[yOutSize] = {};

    interpolateNormalized( createFloatSpan( yInput, sizeof( yInput ) / sizeof( *yInput ) ),
                           createFloatSpan( yOut, yOutSize ) );

    assertFloatContainersEqual( createFloatSpan( expectedOutput, sizeof( expectedOutput ) / sizeof( *expectedOutput ) ),
                                createFloatSpan( yOut, yOutSize ), 0.001f );
}

static bool equalRGB( const ColorRGB_t lhs, const ColorRGB_t rhs )
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

void givenColorsRatio_convertColorsRatioToRGBColors_convertToRGB()
{

    static const size_t colorRatioSize = 3;
    float               colorsRatio[]  = { 0, 0.5, 1 };

    const ColorRGB_t   mainColor            = { 100, 0, 0 };
    const ColorRGB_t   secondaryColor       = { 0, 100, 0 };
    static const float secondaryColorFactor = 0.5f;

    ColorRGB_t       rgbColors[colorRatioSize];
    const ColorRGB_t expectedRgb[] = { { 0, 50, 0 }, { 50, 25, 0 }, { 100, 0, 0 } };

    convertColorsRatioToRGBColors( createFloatSpan( colorsRatio, colorRatioSize ), rgbColors, mainColor, secondaryColor,
                                   secondaryColorFactor );

    for ( uint32_t i = 0; i < colorRatioSize; ++i )
    {
        assert_true( equalRGB( rgbColors[i], expectedRgb[i] ) );
    }
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenBinaryNumber_binaryToArray_returnAnArrayOfBits ),
        cmocka_unit_test( givenTwoIncreasingSamples_interpolateNormalized_interpolate ),
        cmocka_unit_test( givenTwoDecreasingSamples_interpolateNormalized_interpolate ),
        cmocka_unit_test( givenTwoEqualSamples_interpolateNormalized_repeatInputByScale ),
        cmocka_unit_test( givenColorsRatio_convertColorsRatioToRGBColors_convertToRGB ),
    };

    return cmocka_run_group_tests_name( "Picture converter utils tests ", tests, NULL, NULL );
}
