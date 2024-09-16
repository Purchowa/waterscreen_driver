#include "mocked_functions.h"

#include <stdint.h>
#include <setjmp.h>
#include <cmocka.h>

void manageValvePower( DeviceState_t state )
{
    check_expected( state );
}
void manageWaterPump( DeviceState_t state )
{
    check_expected( state );
}

status_t sendDataToValves( const uint64_t *data )
{
    check_expected( *data );

    return mock_type( status_t );
}

bool shouldWaterPumpTrigger()
{
    return mock_type( bool );
}
bool shouldWaterAlaramTrigger()
{
    return mock_type( bool );
}

const PictureDataView_t *getEachPictureView()
{
    return mock_ptr_type( PictureDataView_t * );
}

// Unfortunatelly cmocka doesn't provide functionality for mocking user-defined non-ptr objects.
static Datetime_t mockedDatetime;
static bool       wasMocked = false;

void will_return_datetime( const Datetime_t datetime )
{
    wasMocked      = true;
    mockedDatetime = datetime;
}

static const Datetime_t *get_mocked_datetime()
{
    assert_true( wasMocked );
    wasMocked = false;
    return &mockedDatetime;
}

Datetime_t getRTCDatetime()
{
    return *get_mocked_datetime();
}

WeatherApiStatusCode_t getWeather( Weather_t *weather )
{
    assert_non_null( weather );

    mock_ptr_type( Weather_t * );
    mock_type( WeatherApiStatusCode_t );
}
