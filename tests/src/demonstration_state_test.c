#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "power_control.h"
#include "spi_transfer.h"
#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

#include "utils/common_state_assert.h"

static const uint64_t pictureSample[] = { 0b1000000000000000000000000000000000000000000000000000000000000000,
                                          0b0100000000000000000000000000000000000000000000000000000000000000,
                                          0b0010000000000000000000000000000000000000000000000000000000000000,
                                          0b0001000000000000000000000000000000000000000000000000000000000000 };

static const pictureData_t picture = { .dataBuffer = pictureSample, .rowCount = 4 };

static void givenDemoModeState_demoModeState_transitionToIdleState( void **state )
{
    WaterscreenContext_t context = { .waterscreenStateHandler = demoModeState,
                                     .picture                 = NULL,
                                     .demoLoopCount           = 0,
                                     .valveOpenCounter        = 0,
                                     .currentStateStatus      = Success };

    assertClosedValves();
    performWaterscreenAction( &context );

    assert_ptr_equal( context.waterscreenStateHandler, idleState );
}

static void givenDemoModeStateWithLoopCount_demoModeState_printMultiplePictures( void **state )
{
    static const int32_t endOfDemoCounterValue = -1;
    static const uint8_t lastIndexFromPicture  = 3;
    static const uint8_t loopCount             = 3;

    WaterscreenContext_t context = { .waterscreenStateHandler = demoModeState,
                                     .picture                 = NULL,
                                     .demoLoopCount           = loopCount,
                                     .valveOpenCounter        = 0,
                                     .currentStateStatus      = Success };

    for ( int8_t i = 0; i < loopCount; ++i )
    {
        expect_value( manageValvePower, state, OnDeviceState );
        assert_ptr_equal( context.waterscreenStateHandler, demoModeState );
        will_return( getPicture, &picture );
        performWaterscreenAction( &context );

        assert_ptr_equal( context.picture, &picture );
        assert_int_equal( context.valveOpenCounter, lastIndexFromPicture );

        assert_ptr_equal( context.waterscreenStateHandler, presentationState );
        for ( int8_t j = 0; j <= lastIndexFromPicture; ++j )
        {
            expect_any( sendDataToValves, *data );
            expect_any( manageWaterPump, state );
            will_return( sendDataToValves, Success );
            will_return( shouldWaterAlaramTrigger, false );
            will_return( shouldWaterPumpTrigger, false );
            assert_ptr_equal( context.waterscreenStateHandler, presentationState );
            performWaterscreenAction( &context );
            assert_ptr_equal( context.picture, &picture );
        }
        assert_int_equal( context.valveOpenCounter, endOfDemoCounterValue );
        assert_ptr_equal( context.waterscreenStateHandler, presentationState );
        will_return( shouldWaterAlaramTrigger, false );
        will_return( shouldWaterPumpTrigger, false );
        expect_any( manageWaterPump, state );
        performWaterscreenAction( &context );
    }
    assert_int_equal( context.valveOpenCounter, endOfDemoCounterValue );
    assertClosedValves();
    assert_ptr_equal( context.waterscreenStateHandler, demoModeState );
    performWaterscreenAction( &context );

    assert_ptr_equal( context.waterscreenStateHandler, idleState );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenDemoModeState_demoModeState_transitionToIdleState ),
        cmocka_unit_test( givenDemoModeStateWithLoopCount_demoModeState_printMultiplePictures ),
    };

    return cmocka_run_group_tests_name( "Demonstration State", tests, NULL, NULL );
}
