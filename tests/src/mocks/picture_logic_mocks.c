#include "picture_management/demo_mode_picture_logic.h"

#include "external_communication/weather_types.h"

#include <setjmp.h>
#include <cmocka.h>


const PictureInfo_t *getEachPicture()
{
    return mock_ptr_type( PictureInfo_t * );
}

const PictureDataSpan_t *getOccasionalPicture( const Datetime_t *datetime, const WeatherCondition_t condition )
{
    assert_non_null( datetime );
    assert_true( 0 < condition && condition < WEATHER_CONDITION_SIZE );

    return mock_ptr_type( PictureDataSpan_t * );
}
