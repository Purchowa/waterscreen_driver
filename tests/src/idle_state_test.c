#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "power_control.h"
#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

#include "utils/common_state_assert.h"


void givenPumpOn_idleState_turnOffPump( void **state )
{
    WaterscreenContext_t context = { .waterscreenStateHandler = idleState,
                                     .picture                 = NULL,
                                     .demoLoopCount           = 0,
                                     .valveOpenCounter        = 0,
                                     .currentStateStatus      = Success };

    // Assume that pump is on when entering idle.
    will_return( shouldWaterAlaramTrigger, false );
    will_return( shouldWaterPumpTrigger, true );
    expect_value( manageWaterPump, state, OnDeviceState );
    performWaterscreenAction( &context );

    // When sensors trigger turn off pump
    will_return( shouldWaterAlaramTrigger, false );
    will_return( shouldWaterPumpTrigger, false );
    expect_value( manageWaterPump, state, OffDeviceState );
    performWaterscreenAction( &context );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenPumpOn_idleState_turnOffPump ),
    };

    return cmocka_run_group_tests_name( "Idle State", tests, NULL, NULL );
}
