#include "standard_mode_picture_logic.h"

#include "standard_mode_picture_getters.h"
#include "picture_logic_utils.h"

#include <string.h>
#include <assert.h>


const PictureDataSpan_t *getOccasionalPicture( const Datetime_t *datetime, const WeatherCondition_t weatherCondition )
{
    static size_t s_callCounter = 0;

    PictureDataSpan_t        *picture = NULL;
    PictureGetterLoopStatus_t getterStatus =
        callPictureGetterAtIndex( s_callCounter, &picture, datetime, weatherCondition );

    while ( getterStatus == NoAvailablePicture )
    {
        s_callCounter = cyclicIncrement( s_callCounter, STANDARD_MODE_PICTURE_GETTER_COUNT );
        getterStatus  = callPictureGetterAtIndex( s_callCounter, &picture, datetime, weatherCondition );
    }

    if ( getterStatus == PictureGetterEndLoop )
    {
        s_callCounter = cyclicIncrement( s_callCounter, STANDARD_MODE_PICTURE_GETTER_COUNT );
    }

    return picture;
}
