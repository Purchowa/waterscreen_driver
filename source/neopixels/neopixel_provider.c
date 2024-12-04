#include "neopixel_provider.h"

#include "neopixels/neopixel_converter.h"
#include "neopixels/neopixel_defines.h"
#include "external/neopixel/neopixels.h"

#include <string.h>


static colorGRB_t  s_grbColors[NEOPIXEL_COUNT];
static const float s_secondaryColorFactor = 0.5f;

void dimNeopixels()
{
    memset( s_grbColors, 0, NEOPIXEL_COUNT * sizeof( *s_grbColors ) );
    Neopixels_Send( s_grbColors );
}

void lightUpNeopixels( const pictureRow_t pictureRow, ColorRGB_t mainColor, ColorRGB_t secondaryColor )
{
    convertPictureRowToGRBColors( pictureRow, s_grbColors, mainColor, secondaryColor, s_secondaryColorFactor );
    Neopixels_Send( s_grbColors );
}
