#include "picture_data.h"

#include "picture_management/demo_mode_picture_logic.h"
#include "picture_management/picture_data.h"

#include <assert.h>


const PictureDataView_t *getEachPictureView()
{
    static size_t s_pictureIndex = 0;

    assert( s_pictureIndex < PICTURE_COUNT );
    const PictureDataView_t *currentPictureViewPtr = &g_allPictures[s_pictureIndex];
    s_pictureIndex                                 = ( s_pictureIndex + 1 ) % PICTURE_COUNT;

    return currentPictureViewPtr;
}
