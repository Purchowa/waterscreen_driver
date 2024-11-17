#include "picture_management/picture_logic_utils.h"

#include <string.h>
#include <assert.h>

int32_t getLastPictureIndex( const PictureDataSpan_t *picture )
{
    assert( picture );

    return picture->size - 1;
}

size_t cyclicIncrement( const size_t index, const size_t limit )
{
    return ( index + 1 ) % limit;
}
