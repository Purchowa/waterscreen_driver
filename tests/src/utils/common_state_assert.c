#include "common_state_assert.h"

#include "waterscreen_states.h"

#include "power_control.h"
#include "spi_transfer.h"

#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>


void assertClosedValves()
{
    expect_value( sendDataToValves, *data, 0 );
    will_return( sendDataToValves, SuccessSPI );
}
