#include "picture_logic.h"

#include "picture_data.h"

#define MAX_CUSTOM_PICTURE_LENGTH 64

const pictureDataView_t *getEachPictureView()
{
    static uint8_t pictureIndex = 0;

    const pictureDataView_t *currentPicture = &g_allPictures[pictureIndex];
    pictureIndex                            = ( pictureIndex + 1 ) % PICTURE_COUNT;

    return currentPicture;
}

static uint64_t userCustomPicture[MAX_CUSTOM_PICTURE_LENGTH];

customPictureDataSpan_t *getCustomPictureSpan()
{
    static customPictureDataSpan_t view = {
        .capacity = MAX_CUSTOM_PICTURE_LENGTH, .size = 0, .data = userCustomPicture };

    return &view;
}
