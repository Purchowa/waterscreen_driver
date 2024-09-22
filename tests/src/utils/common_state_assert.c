#include "common_state_assert.h"

#include "waterscreen_states.h"
#include "../mocks/digital_signaling_mocks.h"

#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>


void assertClosedValves()
{
    expect_value( manageValvePower, state, OffDeviceState );
    expect_value( sendDataToValves, *data, 0 );
    will_return( sendDataToValves, SuccessSPI );
}
