#include "datetime_mocks.h"

#include <stdbool.h>
#include <setjmp.h>
#include <cmocka.h>


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
