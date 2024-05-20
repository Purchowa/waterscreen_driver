#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "pictures.h"
#include "power_control.h"
#include "spi_transfer.h"
#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

typedef enum {
    Success,
    InvalidArgument,
    Timeout
} ReturnCodes_t;

const uint64_t pictKI[] = {
        0b1000000000000000000000000000000000000000000000000000000000000000,
        0b0100000000000000000000000000000000000000000000000000000000000000,
        0b0010000000000000000000000000000000000000000000000000000000000000,
        0b0001000000000000000000000000000000000000000000000000000000000000
};

void assignPicture(const pictureData_t* dest) {}

void manageValvePower(DeviceState_t state){}

void manageWaterPump(DeviceState_t state){}

status_t sendDataToValves(const uint64_t* data){return Success;}

static void test_transition_demo_to_idle(void** state)
{
    const pictureData_t picture = { .dataBuffer = pictKI, .rowCount = 4 };
    WaterscreenContext_t context = { .waterscreenStateHandler = demoModeState, .picture=&picture, .valveOpenCounter = 3, .currentStateStatus = Success };

    for (uint32_t i = 0; i < picture.rowCount; ++i) {
        performWaterscreenAction(&context);
    }
    assert_ptr_equal(context.waterscreenStateHandler, choosePictureState);

    performWaterscreenAction(&context);
    assert_ptr_equal(context.waterscreenStateHandler, closeValvesState);

    performWaterscreenAction(&context);
    assert_ptr_equal(context.waterscreenStateHandler, idleState);
}

int main()
{
    const struct CMUnitTest tests[] =
    {
        cmocka_unit_test(test_transition_demo_to_idle),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}