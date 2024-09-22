#include "picture_management/custom_picture_logic.h"
#include "picture_management/picture_data.h"

#include <assert.h>


static uint64_t s_userCustomPicture[MAX_CUSTOM_PICTURE_LENGTH];


CustomPictureDataSpan_t *getCustomPictureSpan()
{
    static CustomPictureDataSpan_t view = {
        .capacity = MAX_CUSTOM_PICTURE_LENGTH, .size = 0, .data = s_userCustomPicture };

    return &view;
}
