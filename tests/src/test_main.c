#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "pictures.h"

#include "power_control.h"
#include "sensors_control.h"
#include "spi_transfer.h"
#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

#include "common_state_assert.h"



static const uint64_t pictKI[] = {
        0b1000000000000000000000000000000000000000000000000000000000000000,
        0b0100000000000000000000000000000000000000000000000000000000000000,
        0b0010000000000000000000000000000000000000000000000000000000000000,
        0b0001000000000000000000000000000000000000000000000000000000000000
};

static const pictureData_t picture = { .dataBuffer = pictKI, .rowCount = 4 };

void assignPicture(const pictureData_t** dest) {
    *dest = &picture;
}


static void givenPicture_demoState_printBottomUp(void** state) {
    WaterscreenContext_t context = { .waterscreenStateHandler = demoModeState, .picture = &picture, .demoLoopCount = 0, .valveOpenCounter = 3, .currentStateStatus = Success };
    for (int8_t i = picture.rowCount - 1; 0 <= i; --i) { // Print picture
        expect_value(sendDataToValves, *data, picture.dataBuffer[i]);
        will_return(shouldWaterPumpTrigger, false);
        will_return(shouldWaterAlaramTrigger, false);
        expect_value(manageWaterPump, state, OffDeviceState);
        will_return(sendDataToValves, Success);
        performWaterscreenAction(&context);
        assert_ptr_equal(context.waterscreenStateHandler, demoModeState);
    }
    assert_ptr_equal(context.waterscreenStateHandler, demoModeState); // Transitioning to different state
}

static void givenAnyState_closeValveSubState_turnValvesOffRemainTheSameState(void** state) {
    WaterscreenContext_t context = { .waterscreenStateHandler = NULL, .picture = NULL, .demoLoopCount = 0, .valveOpenCounter = 0, .currentStateStatus = Success };

    assertClosedValves();
    closeValvesSubState(&context);

    assert_null(context.waterscreenStateHandler); // State handler should remain the same
}

static void givenDemoModeState_demoModeState_transitionToIdleState(void** state) {
    WaterscreenContext_t context = { .waterscreenStateHandler = demoModeState, .picture=&picture, .demoLoopCount = 0, .valveOpenCounter = -1, .currentStateStatus = Success };

    expect_value(manageWaterPump, state, OffDeviceState);
    will_return(shouldWaterPumpTrigger, false);
    will_return(shouldWaterAlaramTrigger, false);
    performWaterscreenAction(&context);

    assertClosedValves();
    assert_ptr_equal(context.waterscreenStateHandler, choosePictureState);
    performWaterscreenAction(&context);
    
    assert_ptr_equal(context.waterscreenStateHandler, idleState);
}

static void givenChoosePictureStateWithLoopCount_demoModeState_printMultiplePictures(void** state) {
    static const int32_t endOfDemoCounterValue = -1;
    static const uint8_t lastIndexFromPicture = 3;
    static const uint8_t loopCount = 3;

    WaterscreenContext_t context = { .waterscreenStateHandler = choosePictureState, .picture = NULL, .demoLoopCount = loopCount, .valveOpenCounter = 0, .currentStateStatus = Success };
    
    for (int8_t i = 0; i < loopCount; ++i) {

        expect_value(manageValvePower, state, OnDeviceState);
        assert_ptr_equal(context.waterscreenStateHandler, choosePictureState);
        performWaterscreenAction(&context);

        assert_non_null(context.picture);
        assert_int_equal(context.valveOpenCounter, lastIndexFromPicture);

        assert_ptr_equal(context.waterscreenStateHandler, demoModeState);
        for (int8_t i = 0; i <= lastIndexFromPicture; ++i) {
            expect_any(sendDataToValves, *data);
            expect_any(manageWaterPump, state);
            will_return(sendDataToValves, Success);
            will_return(shouldWaterAlaramTrigger, false);
            will_return(shouldWaterPumpTrigger, false);
            assert_ptr_equal(context.waterscreenStateHandler, demoModeState);
            performWaterscreenAction(&context);
            assert_non_null(context.picture);
        }
        assert_int_equal(context.valveOpenCounter, endOfDemoCounterValue);
        assert_ptr_equal(context.waterscreenStateHandler, demoModeState);
        will_return(shouldWaterAlaramTrigger, false);
        will_return(shouldWaterPumpTrigger, false);
        expect_any(manageWaterPump, state);
        performWaterscreenAction(&context);
    }
    assert_int_equal(context.valveOpenCounter, endOfDemoCounterValue);
    assertClosedValves();
    assert_ptr_equal(context.waterscreenStateHandler, choosePictureState);
    performWaterscreenAction(&context);

    assert_ptr_equal(context.waterscreenStateHandler, idleState);
}

void givenAlarmOnPumpOff_checkSensorsSubState_transitionToLowWaterStateWithClosedValves(void** state)
{
    WaterscreenContext_t context = { .waterscreenStateHandler = NULL, .picture = NULL, .demoLoopCount = 0, .valveOpenCounter = 0, .currentStateStatus = Success };

    assertClosedValves();
    will_return(shouldWaterAlaramTrigger, true);
    will_return(shouldWaterPumpTrigger, false);
    expect_value(manageWaterPump, state, OffDeviceState);
    checkSensorsSubState(&context);

    assert_ptr_equal(context.waterscreenStateHandler, lowWaterState);
}

void givenAlarmOnPumpOn_checkSensorsSubState_transitionToLowWaterStateWithClosedValves(void** state)
{
    WaterscreenContext_t context = { .waterscreenStateHandler = NULL, .picture = NULL, .demoLoopCount = 0, .valveOpenCounter = 0, .currentStateStatus = Success };

    assertClosedValves();
    will_return(shouldWaterAlaramTrigger, true);
    will_return(shouldWaterPumpTrigger, true);
    expect_value(manageWaterPump, state, OffDeviceState);
    checkSensorsSubState(&context);

    assert_ptr_equal(context.waterscreenStateHandler, lowWaterState);
}

void givenAlarmOffPumpOff_checkSensorsSubState_remainInitialStateWithOpenValves(void** state)
{
    WaterscreenContext_t context = { .waterscreenStateHandler = NULL, .picture = NULL, .demoLoopCount = 0, .valveOpenCounter = 0, .currentStateStatus = Success };

    will_return(shouldWaterAlaramTrigger, false);
    will_return(shouldWaterPumpTrigger, false);
    expect_value(manageWaterPump, state, OffDeviceState);
    checkSensorsSubState(&context);

    assert_null(context.waterscreenStateHandler);
}

void givenAlarmOffPumpOn_checkSensorsSubState_remainInitialStateWithOpenValves(void** state)
{
    WaterscreenContext_t context = { .waterscreenStateHandler = NULL, .picture = NULL, .demoLoopCount = 0, .valveOpenCounter = 0, .currentStateStatus = Success };

    will_return(shouldWaterAlaramTrigger, false);
    will_return(shouldWaterPumpTrigger, true);
    expect_value(manageWaterPump, state, OnDeviceState);
    checkSensorsSubState(&context);

    assert_null(context.waterscreenStateHandler);
}

void givenAlaramOn_lowWaterState_remainLowWaterState(void** state)
{
    WaterscreenContext_t context = { .waterscreenStateHandler = lowWaterState, .picture = NULL, .demoLoopCount = 0, .valveOpenCounter = 0, .currentStateStatus = Success };

    for (uint8_t i = 0; i < 3; ++i) {
        expect_value(manageWaterPump, state, OffDeviceState);
        will_return(shouldWaterAlaramTrigger, true);
        performWaterscreenAction(&context);
        assert_ptr_equal(context.waterscreenStateHandler, lowWaterState);
    }
}

void givenAlarmOff_lowWaterState_changeStateToIdle(void** state)
{
    WaterscreenContext_t context = { .waterscreenStateHandler = lowWaterState, .picture = NULL, .demoLoopCount = 0, .valveOpenCounter = 0, .currentStateStatus = Success };

    expect_value(manageWaterPump, state, OffDeviceState);
    will_return(shouldWaterAlaramTrigger, false);
    performWaterscreenAction(&context);

    assert_ptr_equal(context.waterscreenStateHandler, idleState);
}

void givenPumpOn_lowWaterState_remainAlwaysPumpOff(void** state)
{
    WaterscreenContext_t context = { .waterscreenStateHandler = lowWaterState, .picture = NULL, .demoLoopCount = 0, .valveOpenCounter = 0, .currentStateStatus = Success };

    expect_value(manageWaterPump, state, OffDeviceState);
    will_return(shouldWaterAlaramTrigger, false);
    performWaterscreenAction(&context);
}

void givenPumpOn_idleState_turnOffPump(void** state) 
{
    WaterscreenContext_t context = { .waterscreenStateHandler = idleState, .picture = NULL, .demoLoopCount = 0, .valveOpenCounter = 0, .currentStateStatus = Success };

    // Assume that pump is on when entering idle.
    will_return(shouldWaterAlaramTrigger, false);
    will_return(shouldWaterPumpTrigger, true);
    expect_value(manageWaterPump, state, OnDeviceState);
    performWaterscreenAction(&context);

    // When sensors trigger turn off pump
    will_return(shouldWaterAlaramTrigger, false);
    will_return(shouldWaterPumpTrigger, false);
    expect_value(manageWaterPump, state, OffDeviceState);
    performWaterscreenAction(&context);
}

int main()
{
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(givenAnyState_closeValveSubState_turnValvesOffRemainTheSameState),

        cmocka_unit_test(givenPicture_demoState_printBottomUp),
        cmocka_unit_test(givenDemoModeState_demoModeState_transitionToIdleState),
        cmocka_unit_test(givenChoosePictureStateWithLoopCount_demoModeState_printMultiplePictures),

        cmocka_unit_test(givenAlarmOnPumpOff_checkSensorsSubState_transitionToLowWaterStateWithClosedValves),
        cmocka_unit_test(givenAlarmOnPumpOn_checkSensorsSubState_transitionToLowWaterStateWithClosedValves),
        cmocka_unit_test(givenAlarmOffPumpOff_checkSensorsSubState_remainInitialStateWithOpenValves),
        cmocka_unit_test(givenAlarmOffPumpOn_checkSensorsSubState_remainInitialStateWithOpenValves),

        cmocka_unit_test(givenAlaramOn_lowWaterState_remainLowWaterState),
        cmocka_unit_test(givenAlarmOff_lowWaterState_changeStateToIdle),
        cmocka_unit_test(givenPumpOn_lowWaterState_remainAlwaysPumpOff),

        cmocka_unit_test(givenPumpOn_idleState_turnOffPump),
    };

    return cmocka_run_group_tests_name("Waterscreen machine state tests", tests, NULL, NULL);
}