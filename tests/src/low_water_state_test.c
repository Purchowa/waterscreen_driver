#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "power_control.h"
#include "spi_transfer.h"
#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

#include "utils/common_state_assert.h"

void givenAlaramOn_lowWaterState_remainLowWaterState( void **state )
{
    WaterscreenContext_t context = { .waterscreenStateHandler = lowWaterState,
                                     .picture                 = NULL,
                                     .valveOpenCounter        = 0,
                                     .currentStateStatus      = Success };

    const uint8_t arbitraryLoopCountInLowWaterState = 3;
    for ( uint8_t i = 0; i < arbitraryLoopCountInLowWaterState; ++i )
    {
        expect_value( manageWaterPump, state, OffDeviceState );
        will_return( shouldWaterAlaramTrigger, true );
        performWaterscreenAction( &context );
        assert_ptr_equal( context.waterscreenStateHandler, lowWaterState );
    }
}

void givenAlarmOff_lowWaterState_changeStateToIdle( void **state )
{
    WaterscreenContext_t context = { .waterscreenStateHandler = lowWaterState,
                                     .picture                 = NULL,
                                     .valveOpenCounter        = 0,
                                     .currentStateStatus      = Success };

    expect_value( manageWaterPump, state, OffDeviceState );
    will_return( shouldWaterAlaramTrigger, false );
    performWaterscreenAction( &context );

    assert_ptr_equal( context.waterscreenStateHandler, idleState );
}

int main()
{
    const struct CMUnitTest tests[] = { cmocka_unit_test( givenAlaramOn_lowWaterState_remainLowWaterState ),
                                        cmocka_unit_test( givenAlarmOff_lowWaterState_changeStateToIdle ) };

    return cmocka_run_group_tests_name( "Low-water State", tests, NULL, NULL );
}
