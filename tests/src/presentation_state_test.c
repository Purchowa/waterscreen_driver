#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "picture_management/demo_mode_picture_logic.h"

#include "power_control.h"
#include "waterscreen_state_context_handler.h"
#include "waterscreen_states.h"

#include "utils/common_state_assert.h"

static const pictureRow_t pictureSample[] = { 0b1000000000000000000000000000000000000000000000000000000000000000,
                                              0b0100000000000000000000000000000000000000000000000000000000000000,
                                              0b0010000000000000000000000000000000000000000000000000000000000000,
                                              0b0001000000000000000000000000000000000000000000000000000000000000 };

static const PictureInfo_t picture = { .picture = { .size = 4, .data = pictureSample } };

static void givenPicture_presentationState_printBottomUp()
{
    will_return( getEachPicture, &picture );
    const PictureInfo_t *mockedPicture = getEachPicture();

    WaterscreenContext_t context = { .waterscreenStateHandler = presentationState,
                                     .pictureInfo             = mockedPicture,
                                     .valveOpenCounter        = mockedPicture->picture.size - 1 };
    for ( int8_t i = mockedPicture->picture.size - 1; 0 <= i; --i )
    { // Print picture
        expect_value( sendDataToValves, *data, mockedPicture->picture.data[i] );
        will_return( shouldWaterPumpTrigger, false );
        will_return( shouldWaterAlarmTrigger, false );
        expect_value( manageWaterPump, state, OffDeviceState );
        will_return( sendDataToValves, SuccessSPI );
        performWaterscreenAction( &context );
        assert_ptr_equal( context.waterscreenStateHandler, presentationState );
    }
}

static void givenValveOpenCounterLessThanZero_presentationState_closeValvesAndGoBackToPreviousState()
{
    WaterscreenContext_t context = {
        .waterscreenStateHandler         = presentationState,
        .previousWaterscreenStateHandler = demoModeState,
        .valveOpenCounter                = -1,
    };

    will_return( shouldWaterPumpTrigger, false );
    will_return( shouldWaterAlarmTrigger, false );
    expect_value( manageWaterPump, state, OffDeviceState );

    assertClosedValves();
    performWaterscreenAction( &context );

    assert_ptr_equal( context.waterscreenStateHandler, demoModeState );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenPicture_presentationState_printBottomUp ),
        cmocka_unit_test( givenValveOpenCounterLessThanZero_presentationState_closeValvesAndGoBackToPreviousState ),
    };

    return cmocka_run_group_tests_name( "Presentation State", tests, NULL, NULL );
}
