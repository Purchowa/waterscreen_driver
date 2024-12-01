
#include <assert.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "neopixel_defines.h"
#include "neopixel_converter.h"


static const size_t expectedNeopixelCount = 12;
static const size_t expectedPictureWidth  = 6;

void test()
{
    const pictureRow_t binRow = 0b101100;

    colorGRB_t       grbColors[NEOPIXEL_COUNT] = { 0 };
    const ColorRGB_t mainColor                 = { 255, 0, 0 };
    const ColorRGB_t secondaryColor            = { 0, 255, 0 };
    const float      secondaryColorFactor      = 0.5f;

    convertPictureRowToGRBColors( binRow, grbColors, mainColor, secondaryColor, secondaryColorFactor );

    const uint32_t   scale        = 2;
    const colorGRB_t expectedMain = 0x00ff00;

    static const size_t G_SHIFT = 16;
    // Two steps assuming scale == 2;
    const colorGRB_t expectedSecondary = (uint32_t)( secondaryColor.g * secondaryColorFactor ) << G_SHIFT;
    const colorGRB_t expectedStep      = ( expectedMain + expectedSecondary ) / scale;

    const colorGRB_t expectedGrbColors[NEOPIXEL_COUNT] = { expectedMain,      // 1
                                                           expectedStep,
                                                           expectedSecondary, // 0
                                                           expectedStep,
                                                           expectedMain,      // 1
                                                           expectedMain,
                                                           expectedMain,      // 1
                                                           expectedStep,
                                                           expectedSecondary, // 0
                                                           expectedSecondary,
                                                           expectedSecondary, // 0
                                                           expectedSecondary };


    static colorGRB_t eps = 35000; // due to flot to int casting.
    for ( size_t i = 0; i < NEOPIXEL_COUNT; ++i )
    {
        assert_in_range( grbColors[i], expectedGrbColors[i] - eps, expectedGrbColors[i] + eps );
    }
}

int assertProperSize()
{
    if ( NEOPIXEL_COUNT != expectedNeopixelCount || PICTURE_WIDTH != expectedPictureWidth )
    {
        fail_msg( "Test expects different macro values. Check CMake target_compile_definitions" );
        return 1;
    }

    return 0;
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup( test, assertProperSize ),
    };

    return cmocka_run_group_tests_name( "Picture converter integration tests ", tests, NULL, NULL );
}
