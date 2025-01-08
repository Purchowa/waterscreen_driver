#include "neopixel_provider.h"

#include "neopixels/neopixel_converter.h"
#include "neopixels/neopixel_defines.h"
#include "external/neopixel/neopixels.h"

#include <string.h>

static colorGRB_t  s_grbColors[TOTAL_NEOPIXEL_COUNT];
static const float s_secondaryColorFactor = 0.1f;

void dimNeopixels()
{
    memset( s_grbColors, 0, TOTAL_NEOPIXEL_COUNT * sizeof( *s_grbColors ) );
    Neopixels_Send( s_grbColors );
}

void lightUpNeopixels( const pictureRow_t pictureRow, ColorRGB_t mainColor, ColorRGB_t secondaryColor )
{
    convertPictureRowToGRBColors( pictureRow, s_grbColors, mainColor, secondaryColor, s_secondaryColorFactor );
    memcpy( &s_grbColors[NEOPIXEL_COUNT], s_grbColors, NEOPIXEL_COUNT * sizeof( *s_grbColors ) );
    Neopixels_Send( s_grbColors );
}
