#include <assert.h>
#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "color.h"

void givenRgbColor_rgbToGrb_convertToGrbInt()
{
    const uint32_t   expectedGrbRed = 0x00ff00;
    const ColorRGB_t rgbRed         = { 255, 0, 0 };

    assert_int_equal( expectedGrbRed, rgbToGrb( rgbRed ) );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenRgbColor_rgbToGrb_convertToGrbInt ),
    };

    return cmocka_run_group_tests_name( "Colors conversion tests", tests, NULL, NULL );
}
