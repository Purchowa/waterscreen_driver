#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "neopixel_provider.h"
#include "converter_utils.h"

ColorRGB_t adjustColorBrightness( ColorRGB_t color, float factor )
{
    ColorRGB_t rgb = {};
    return rgb;
};

void lightUpNeopixelsWithColor( ColorRGB_t color )
{
    function_called();
};

void lightUpNeopixels( const pictureRow_t pictureRow, ColorRGB_t mainColor, ColorRGB_t secondaryColor )
{
    check_expected( pictureRow );
}

void dimNeopixels() {}
