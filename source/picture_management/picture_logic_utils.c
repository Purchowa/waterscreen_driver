#include "picture_management/picture_logic_utils.h"

#include <assert.h>

uint8_t getLastPictureIndex( const PictureDataView_t *picture )
{
    assert( picture );

    return picture->size - 1;
}
