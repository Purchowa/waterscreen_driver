#include "neopixel_provider.h"

#include "neopixels/converter_utils.h"
#include "neopixels/neopixel_converter.h"
#include "neopixels/neopixel_defines.h"
#include "external/neopixel/neopixels.h"

#include <string.h>

static colorGRB_t s_grbColors[TOTAL_NEOPIXEL_COUNT];

void dimNeopixels()
{
    memset( s_grbColors, 0, TOTAL_NEOPIXEL_COUNT * sizeof( *s_grbColors ) );
    Neopixels_Send( s_grbColors );
}

void lightUpNeopixels( const pictureRow_t pictureRow, ColorRGB_t mainColor, ColorRGB_t secondaryColor )
{
    convertPictureRowToGRBColors( pictureRow, s_grbColors, mainColor, secondaryColor, MAIN_COLOR_FACTOR,
                                  SECONDARY_COLOR_FACTOR );
    copyReverse( &s_grbColors[NEOPIXEL_COUNT], s_grbColors, NEOPIXEL_COUNT );
    Neopixels_Send( s_grbColors );
}

void lightUpNeopixelsWithColor( ColorRGB_t color )
{
    memset( s_grbColors, rgbToGrb( color ), TOTAL_NEOPIXEL_COUNT * sizeof( *s_grbColors ) );
    Neopixels_Send( s_grbColors );
}
