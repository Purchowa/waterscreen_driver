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

const pictureDataView_t *getPictureView()
{
    return mock_ptr_type( pictureDataView_t * );
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
