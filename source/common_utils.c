#include "common_utils.h"


int32_t clamp( const int32_t value, const int32_t min, const int32_t max )
{
    const int32_t lower = value < min ? min : value;
    return max < lower ? max : lower;
}
