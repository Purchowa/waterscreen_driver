#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "mocks/datetime_mocks.h"

#include "waterscreen_state/waterscreen_state_context_handler.h"
#include "waterscreen_state/standard_mode_state.h"
#include "picture_management/standard_mode_picture_logic.h"
#include "datetime/datetime_types.h"
#include "gpio/power_control.h"

#include <sensors_control.h>
#include <waterscreen_states.h>

static void checkSensorsMock()
{
    will_return( shouldWaterAlarmTrigger, false );
    will_return( shouldWaterPumpTrigger, false );
    expect_any( manageWaterPump, state );
}

static pictureRow_t            s_mockedSingleRow = 128;
static const PictureDataSpan_t s_expectedPicture = { .size = 1, .data = &s_mockedSingleRow };

void givenWeekendWithoutEnabledWeekends_standardModeState_turnOffValvePowerAndCheckSensors()
{
    const StandardModeConfig_t cfg = { .isWorkingDuringWeekends = false,
                                       .workTimeInStandardMode  = 1,
                                       .idleTimeInStandardMode  = 1,
                                       .workRange               = { .from = 7, .to = 18 } };
    initStandardModeConfig( &cfg );

    WaterscreenContext_t context  = { .waterscreenStateHandler = standardModeState };
    const Datetime_t     datetime = { .date = { .year = 2024, .month = September, .day = 7, .weekday = Saturday } };

    will_return_datetime( datetime );
    expect_value( manageValvePower, state, OffDeviceState );
    checkSensorsMock();
    performWaterscreenAction( &context );

    assert_ptr_equal( context.waterscreenStateHandler, standardModeState );
}

void givenWeekWithEnabledWeekendsAndInWorkRange_standardModeState_getPictureAndChangeToPresentationState()
{
    const StandardModeConfig_t cfg = { .isWorkingDuringWeekends = true,
                                       .workTimeInStandardMode  = 3,
                                       .idleTimeInStandardMode  = 2,
                                       .workRange               = { .from = 7, .to = 18 } };
    initStandardModeConfig( &cfg );

    WaterscreenContext_t context  = { .waterscreenStateHandler = standardModeState };
    const Datetime_t     datetime = { .date = { .year = 2024, .month = September, .day = 6, .weekday = Friday },
                                      .time = { .hour = 7, .minute = 5, .second = 5 } };

    const Weather_t expectedWeather = { .temperature = 31, .pressure = 1024, .weatherCondition = Clear };

    will_return_datetime( datetime );
    will_return( getWeather, &expectedWeather );
    will_return( getOccasionalPicture, &s_expectedPicture );
    expect_value( manageValvePower, state, OnDeviceState );
    performWaterscreenAction( &context );

    assert_ptr_equal( context.waterscreenStateHandler, presentationState );
}

void givenWeekWithEnabledWeekendsOutsideOfWorkRange_standardModeState_turnOffValvePowerAndCheckSensors()
{
    const StandardModeConfig_t cfg = { .isWorkingDuringWeekends = true,
                                       .workTimeInStandardMode  = 1,
                                       .idleTimeInStandardMode  = 1,
                                       .workRange               = { .from = 7, .to = 18 } };
    initStandardModeConfig( &cfg );

    WaterscreenContext_t context  = { .waterscreenStateHandler = standardModeState };
    const Datetime_t     datetime = { .date = { .year = 2024, .month = September, .day = 6, .weekday = Friday },
                                      .time = { .hour = 20, .minute = 0, .second = 0 } };

    will_return_datetime( datetime );
    expect_value( manageValvePower, state, OffDeviceState );
    checkSensorsMock();
    performWaterscreenAction( &context );

    assert_ptr_equal( context.waterscreenStateHandler, standardModeState );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenWeekendWithoutEnabledWeekends_standardModeState_turnOffValvePowerAndCheckSensors ),
        cmocka_unit_test(
            givenWeekWithEnabledWeekendsAndInWorkRange_standardModeState_getPictureAndChangeToPresentationState ),
        cmocka_unit_test(
            givenWeekWithEnabledWeekendsOutsideOfWorkRange_standardModeState_turnOffValvePowerAndCheckSensors ),
    };

    return cmocka_run_group_tests_name( "Standard mode state test ", tests, NULL, NULL );
}
