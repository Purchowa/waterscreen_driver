#include "weather_api.h"

#include <setjmp.h>
#include <cmocka.h>

#include "picture_management/demo_mode_picture_logic.h"

WeatherApiStatusCode_t getWeather( Weather_t *weather )
{
    assert_non_null( weather );

    mock_ptr_type( Weather_t * );
    mock_type( WeatherApiStatusCode_t );
}
