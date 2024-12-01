#include "color.h"

#define R_SHIFT 8
#define G_SHIFT 16


colorGRB_t rgbToGrb( const ColorRGB_t color )
{
    return ( ( (uint32_t)color.g << G_SHIFT ) | ( (uint32_t)color.r << R_SHIFT ) | (uint32_t)color.b );
}
