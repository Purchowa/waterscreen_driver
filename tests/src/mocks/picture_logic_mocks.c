#include "picture_management/demo_mode_picture_logic.h"

#include <setjmp.h>
#include <cmocka.h>


const PictureDataView_t *getEachPictureView()
{
    return mock_ptr_type( PictureDataView_t * );
}

const PictureDataView_t *getOccasionalPictureView( const Datetime_t *datetime, const WeatherCondition_t condition )
{
    assert_non_null( datetime );
    assert_true( 0 < condition && condition < WEATHER_CONDITION_SIZE );

    return mock_ptr_type( PictureDataView_t * );
}
