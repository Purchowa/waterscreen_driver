#include "picture_data.h"

#include "demo_mode_picture_logic.h"
#include "picture_data.h"
#include "picture_logic_utils.h"

#include <assert.h>


const PictureDataSpan_t *getEachPicture()
{
    static size_t s_pictureIndex = 0;

    const PictureDataSpan_t *currentPicture = &g_allPictures[s_pictureIndex];
    s_pictureIndex                          = cyclicIncrement( s_pictureIndex, ALL_PICTURE_COUNT );

    return currentPicture;
}
