#include "picture_data.h"
#include <string.h>
#include <assert.h>
#include <picture_management/picture_data.h>
#include <picture_management/standard_mode_picture_getters.h>
#include <picture_management/standard_mode_picture_logic.h>


const PictureDataView_t *getOccasionalPictureView( const Datetime_t        *datetime,
                                                   const WeatherCondition_t weatherCondition )
{
    static size_t s_callCounter = 0;

    assert( s_callCounter < STANDARD_MODE_PICTURE_GETTER_COUNT );
    const pictureGetterFun_t pictureGetterFun = g_pictureGetterFunctions[s_callCounter];

    const PictureDataView_t        *pictureView  = NULL;
    const PictureGetterLoopStatus_t getterStatus = pictureGetterFun( &pictureView, datetime, weatherCondition );

    if ( getterStatus == PictureGetterEndLoop )
    {
        s_callCounter = ( s_callCounter + 1 ) % STANDARD_MODE_PICTURE_GETTER_COUNT;
    }

    return pictureView;
}
