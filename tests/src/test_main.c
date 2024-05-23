#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "pictures.h"
#include "power_control.h"
#include "spi_transfer.h"
#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

#include "common_state_assert.h"

typedef enum {
    Success,
    InvalidArgument,
    Timeout
} ReturnCodes_t;

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

void manageValvePower(DeviceState_t state){
    check_expected(state);
}

void manageWaterPump(DeviceState_t state){
    check_expected(state);
}

status_t sendDataToValves(const uint64_t* data){
    check_expected(*data);

    return mock_type(status_t);
}

static void test_demo_should_print_bottom_up(void** state) {
    WaterscreenContext_t context = { .waterscreenStateHandler = demoModeState, .picture = &picture, .demoLoopCount = 0, .valveOpenCounter = 3, .currentStateStatus = Success };
    for (int8_t i = picture.rowCount - 1; 0 <= i; --i) { // Print picture
        expect_value(sendDataToValves, *data, picture.dataBuffer[i]);
        will_return(sendDataToValves, Success);
        performWaterscreenAction(&context);
        assert_ptr_equal(context.waterscreenStateHandler, demoModeState);
    }
    assert_ptr_equal(context.waterscreenStateHandler, demoModeState); // Transitioning to different state
}

static void test_transition_demo_to_idle(void** state) {
    WaterscreenContext_t context = { .waterscreenStateHandler = demoModeState, .picture=&picture, .demoLoopCount = 0, .valveOpenCounter = -1, .currentStateStatus = Success };

    performWaterscreenAction(&context);

    expect_value(manageValvePower, state, OffDeviceState);
    expect_value(sendDataToValves, *data, 0);
    will_return(sendDataToValves, Success);

    assert_ptr_equal(context.waterscreenStateHandler, choosePictureState);
    performWaterscreenAction(&context);

    assert_ptr_equal(context.waterscreenStateHandler, closeValvesState);
    performWaterscreenAction(&context);
    
    assert_ptr_equal(context.waterscreenStateHandler, idleState);
}

static void test_not_stuct_in_demo(void** state) {
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
            will_return(sendDataToValves, Success);
            assert_ptr_equal(context.waterscreenStateHandler, demoModeState);
            performWaterscreenAction(&context);
            assert_non_null(context.picture);
        }
        assert_int_equal(context.valveOpenCounter, endOfDemoCounterValue);
        assert_ptr_equal(context.waterscreenStateHandler, demoModeState);
        performWaterscreenAction(&context);
    }
    assert_int_equal(context.valveOpenCounter, endOfDemoCounterValue);
    assert_ptr_equal(context.waterscreenStateHandler, choosePictureState);
    performWaterscreenAction(&context);

    expect_value(manageValvePower, state, OffDeviceState);
    expect_value(sendDataToValves, *data, 0);
    will_return(sendDataToValves, Success);
    assert_ptr_equal(context.waterscreenStateHandler, closeValvesState);
    performWaterscreenAction(&context);

    assert_ptr_equal(context.waterscreenStateHandler, idleState);
    
}

int main()
{
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(test_demo_should_print_bottom_up),
        cmocka_unit_test(test_transition_demo_to_idle),
        cmocka_unit_test(test_not_stuct_in_demo)
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}