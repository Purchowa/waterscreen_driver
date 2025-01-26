#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "picture_management/demo_mode_picture_logic.h"

#include "power_control.h"
#include "waterscreen_state_context_handler.h"
#include "waterscreen_states.c" // Only for test purposes

#include "utils/common_state_assert.h"

static const pictureRow_t pictureSample[] = { 0b1000000000000000000000000000000000000000000000000000000000000000,
                                              0b0100000000000000000000000000000000000000000000000000000000000000,
                                              0b0010000000000000000000000000000000000000000000000000000000000000,
                                              0b0001000000000000000000000000000000000000000000000000000000000000 };


static void givenPictureWithoutRowBitSum_presentationState_printBottomUpWithSameValueForValvesAndNeopixels()
{
    const PictureInfo_t pictInfo = { .picture = { .size = 4, .data = pictureSample }, .enableRowBitSum = false };

    WaterscreenContext_t context = { .waterscreenStateHandler = presentationState,
                                     .pictureInfo             = &pictInfo,
                                     .valveOpenCounter        = pictInfo.picture.size - 1 };
    for ( int8_t i = pictInfo.picture.size - 1; 0 <= i; --i )
    { // Print picture
        expect_value( sendDataToValves, data, pictInfo.picture.data[i] );
        expect_value( lightUpNeopixels, pictureRow, pictInfo.picture.data[i] );
        will_return( shouldWaterPumpTrigger, false );
        will_return( shouldWaterAlarmTrigger, false );
        expect_value( manageWaterPump, state, OffDeviceState );
        will_return( sendDataToValves, SuccessSPI );
        performWaterscreenAction( &context );
        assert_ptr_equal( context.waterscreenStateHandler, presentationState );
    }
}

static void givenPictureWithRowBitSum_presentationState_printBottomUpAccumulateBitsForNeopixelRowAndResetRow()
{
    const PictureInfo_t pictInfo = { .picture = { .size = 4, .data = pictureSample }, .enableRowBitSum = true };

    const pictureRow_t expectedNeopixelRows[] = { 0b1111000000000000000000000000000000000000000000000000000000000000,
                                                  0b0111000000000000000000000000000000000000000000000000000000000000,
                                                  0b0011000000000000000000000000000000000000000000000000000000000000,
                                                  0b0001000000000000000000000000000000000000000000000000000000000000 };

    WaterscreenContext_t context = { .waterscreenStateHandler         = presentationState,
                                     .previousWaterscreenStateHandler = demoModeState,
                                     .pictureInfo                     = &pictInfo,
                                     .valveOpenCounter                = pictInfo.picture.size - 1 };
    for ( int8_t i = pictInfo.picture.size - 1; 0 <= i; --i )
    { // Print picture
        expect_value( sendDataToValves, data, pictInfo.picture.data[i] );
        expect_value( lightUpNeopixels, pictureRow, expectedNeopixelRows[i] );
        will_return( shouldWaterPumpTrigger, false );
        will_return( shouldWaterAlarmTrigger, false );
        expect_value( manageWaterPump, state, OffDeviceState );
        will_return( sendDataToValves, SuccessSPI );
        performWaterscreenAction( &context );
        assert_ptr_equal( context.waterscreenStateHandler, presentationState );
    }

    will_return( shouldWaterPumpTrigger, false );
    will_return( shouldWaterAlarmTrigger, false );
    expect_value( manageWaterPump, state, OffDeviceState );

    expect_value( lightUpNeopixels, pictureRow, expectedNeopixelRows[0] );
    assertClosedValves();
    performWaterscreenAction( &context );
    assert_int_equal( s_neopixelRowData, 0 );
    assert_ptr_equal( context.waterscreenStateHandler, demoModeState );
}

static void givenValveOpenCounterLessThanZero_presentationState_closeValvesAndGoBackToPreviousState()
{
    const PictureInfo_t pictInfo = { .picture = { .size = 4, .data = pictureSample }, .enableRowBitSum = false };

    WaterscreenContext_t context = {
        .waterscreenStateHandler         = presentationState,
        .previousWaterscreenStateHandler = demoModeState,
        .pictureInfo                     = &pictInfo,
        .valveOpenCounter                = -1,
    };

    will_return( shouldWaterPumpTrigger, false );
    will_return( shouldWaterAlarmTrigger, false );
    expect_value( manageWaterPump, state, OffDeviceState );

    expect_function_call( lightUpNeopixelsWithColor );
    assertClosedValves();
    performWaterscreenAction( &context );

    assert_ptr_equal( context.waterscreenStateHandler, demoModeState );
}

static int setupNeopixelRowData()
{
    s_neopixelRowData = 0;

    return 0;
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(

            givenPictureWithoutRowBitSum_presentationState_printBottomUpWithSameValueForValvesAndNeopixels,
            setupNeopixelRowData ),
        cmocka_unit_test_setup(
            givenPictureWithRowBitSum_presentationState_printBottomUpAccumulateBitsForNeopixelRowAndResetRow,
            setupNeopixelRowData ),
        cmocka_unit_test_setup( givenValveOpenCounterLessThanZero_presentationState_closeValvesAndGoBackToPreviousState,
                                setupNeopixelRowData ),
    };

    return cmocka_run_group_tests_name( "Presentation State test", tests, NULL, NULL );
}
