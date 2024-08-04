#include "mocked_functions.h"

#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
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

static const uint64_t pictKI[] = { 0b1000000000000000000000000000000000000000000000000000000000000000,
                                   0b0100000000000000000000000000000000000000000000000000000000000000,
                                   0b0010000000000000000000000000000000000000000000000000000000000000,
                                   0b0001000000000000000000000000000000000000000000000000000000000000 };

static const pictureData_t picture = { .dataBuffer = pictKI, .rowCount = 4 };

void assignPicture( const pictureData_t **dest )
{
    *dest = &picture;
}
