#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "power_control.h"
#include "spi_transfer.h"
#include "waterscreen_state_context_handler.h"
#include "waterscreen_states.h"


void givenAlaramOn_lowWaterState_remainLowWaterState( void **state )
{
    WaterscreenContext_t context = { .waterscreenStateHandler = lowWaterState };

    const uint8_t arbitraryLoopCountInLowWaterState = 3;
    for ( uint8_t i = 0; i < arbitraryLoopCountInLowWaterState; ++i )
    {
        expect_value( manageWaterPump, state, OffDeviceState );
        will_return( shouldWaterAlarmTrigger, true );
        performWaterscreenAction( &context );
        assert_ptr_equal( context.waterscreenStateHandler, lowWaterState );
    }
}

void givenAlarmOff_lowWaterState_changeToPreviousState( void **state )
{
    WaterscreenContext_t context = { .waterscreenStateHandler         = lowWaterState,
                                     .previousWaterscreenStateHandler = demoModeState };

    expect_value( manageWaterPump, state, OffDeviceState );
    will_return( shouldWaterAlarmTrigger, false );
    performWaterscreenAction( &context );

    assert_ptr_equal( context.waterscreenStateHandler, demoModeState );
}

int main()
{
    const struct CMUnitTest tests[] = { cmocka_unit_test( givenAlaramOn_lowWaterState_remainLowWaterState ),
                                        cmocka_unit_test( givenAlarmOff_lowWaterState_changeToPreviousState ) };

    return cmocka_run_group_tests_name( "Low-water State", tests, NULL, NULL );
}
