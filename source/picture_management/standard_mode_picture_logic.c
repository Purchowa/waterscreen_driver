#include "standard_mode_picture_logic.h"

#include "picture_data.h"
#include "standard_mode_picture_getters.h"

#include <string.h>
#include <assert.h>


static void incrementCallCounter( size_t *callCounter )
{
    *callCounter = ( *callCounter + 1 ) % STANDARD_MODE_PICTURE_GETTER_COUNT;
}

const PictureDataView_t *getOccasionalPictureView( const Datetime_t        *datetime,
                                                   const WeatherCondition_t weatherCondition )
{
    static size_t s_callCounter = 0;

    assert( s_callCounter < STANDARD_MODE_PICTURE_GETTER_COUNT );
    const pictureGetterFun_t pictureGetterFun = g_pictureGetterFunctions[s_callCounter];

    const PictureDataView_t  *pictureView = NULL;
    PictureGetterLoopStatus_t getterStatus;
    do
    {
        getterStatus = pictureGetterFun( &pictureView, datetime, weatherCondition );
        incrementCallCounter( &s_callCounter );
    }
    while ( getterStatus == NoAvailablePicture );

    if ( getterStatus == PictureGetterEndLoop )
    {
        incrementCallCounter( &s_callCounter );
    }

    return pictureView;
}
