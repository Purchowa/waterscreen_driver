#include "weather_api.h"

#include <setjmp.h>
#include <cmocka.h>

#include "external_communication/weather_types.h"

const Weather_t *getWeather()
{
    return mock_ptr_type( Weather_t * );
}
