#include "weather_mocks.h"

#include <setjmp.h>
#include <cmocka.h>


WeatherApiStatusCode_t getWeather( Weather_t *weather )
{
    assert_non_null( weather );

    mock_ptr_type( Weather_t * );
    mock_type( WeatherApiStatusCode_t );
}