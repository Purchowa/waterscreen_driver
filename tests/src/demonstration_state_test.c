#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "power_control.h"
#include "spi_transfer.h"
#include "waterscreen_state_context_handler.h"
#include "waterscreen_states.h"
#include "picture_management/picture_logic_utils.h"

#include "utils/common_state_assert.h"

static const pictureRow_t pictureSample[] = { 0b1000000000000000000000000000000000000000000000000000000000000000,
                                              0b0100000000000000000000000000000000000000000000000000000000000000,
                                              0b0010000000000000000000000000000000000000000000000000000000000000,
                                              0b0001000000000000000000000000000000000000000000000000000000000000 };

static const PictureInfo_t picture = { .picture = { .size = 4, .data = pictureSample } };

static void givenDemoModeState_demoModeState_printInfinitelyManyPictures()
{
    static const int32_t endOfDemoCounterValue   = -1;
    static const uint8_t lastIndexFromPicture    = 3;
    static const uint8_t mockedInfiniteLoopCount = 32;

    WaterscreenContext_t context = { .waterscreenStateHandler = demoModeState,
                                     .pictureInfo             = NULL,
                                     .valveOpenCounter        = 0,
                                     .stateStatus             = SuccessSPI };

    for ( int8_t i = 0; i < mockedInfiniteLoopCount; ++i )
    {
        expect_value( manageValvePower, state, OnDeviceState );
        assert_ptr_equal( context.waterscreenStateHandler, demoModeState );
        will_return( getEachPicture, &picture );
        performWaterscreenAction( &context );

        assert_ptr_equal( context.pictureInfo, &picture );
        assert_int_equal( context.valveOpenCounter, lastIndexFromPicture );

        assert_ptr_equal( context.waterscreenStateHandler, presentationState );
        for ( int8_t j = 0; j <= lastIndexFromPicture; ++j )
        {
            expect_any( sendDataToValves, data );
            expect_any( manageWaterPump, state );
            will_return( sendDataToValves, SuccessSPI );
            will_return( shouldWaterAlarmTrigger, false );
            will_return( shouldWaterPumpTrigger, false );
            assert_ptr_equal( context.waterscreenStateHandler, presentationState );
            performWaterscreenAction( &context );
            assert_ptr_equal( context.pictureInfo, &picture );
        }
        assert_int_equal( context.valveOpenCounter, endOfDemoCounterValue );
        assert_ptr_equal( context.waterscreenStateHandler, presentationState );
        will_return( shouldWaterAlarmTrigger, false );
        will_return( shouldWaterPumpTrigger, false );
        expect_any( manageWaterPump, state );
        assertClosedValves();
        expect_function_call( lightUpNeopixelsWithColor );
        performWaterscreenAction( &context );
    }
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenDemoModeState_demoModeState_printInfinitelyManyPictures ),
    };

    return cmocka_run_group_tests_name( "Demonstration State", tests, NULL, NULL );
}
