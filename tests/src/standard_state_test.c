#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "mocks/datetime_mocks.h"

#include "waterscreen_state/standard_mode_state.h"
#include "datetime/datetime_types.h"

void givenWeekendWithoutEnabledWeekends_standardModeState_doNothing()
{
    WaterscreenContext_t context  = { .waterscreenStateHandler = standardModeState };
    const Datetime_t     datetime = { .date = { .year = 2024, .month = 9, .day = 7, .weekday = Saturday },
                                      .time = { .hour = 0, .minute = 0, .second = 0 } };

    will_return_datetime( datetime );
    performWaterscreenAction( &context );

    assert_ptr_equal( context.waterscreenStateHandler, standardModeState );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenWeekendWithoutEnabledWeekends_standardModeState_doNothing ) };

    return cmocka_run_group_tests_name( "Standard mode state test ", tests, NULL, NULL );
}
