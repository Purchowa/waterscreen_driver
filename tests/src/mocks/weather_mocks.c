#include "weather_api.h"

#include <setjmp.h>
#include <cmocka.h>

#include "external_communication/weather_types.h"

HttpReturnCodes_t getWeather( Weather_t *weather )
{
    assert_non_null( weather );

    *weather = *mock_ptr_type( Weather_t * );
    return mock_type( HttpReturnCodes_t );
}
