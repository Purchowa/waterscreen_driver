#include "converter_utils.h"

#include <assert.h>
#include <math.h>

#define BITS_IN_BYTE 8

void binaryToArray( const pictureRow_t binaryRow, byte_t *dstArray, const size_t bitArraySize )
{
    assert( sizeof( binaryRow ) * BITS_IN_BYTE == bitArraySize );

    for ( int32_t i = bitArraySize - 1, bit = 0; 0 <= i; --i, ++bit )
    {
        dstArray[i] = ( binaryRow >> bit ) & 0xb1; // mask to get last bit
    }
}
