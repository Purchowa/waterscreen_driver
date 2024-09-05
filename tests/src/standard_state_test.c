#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "utils/mocked_functions.h"

#include "waterscreen_state_context.h"
#include "waterscreen_state/waterscreen_states.h"

void givenWeekendWithoutEnabledWeekends_standardModeState_doNothing()
{
    WaterscreenContext_t context  = { .waterscreenStateHandler = standardModeState };
    const Datetime_t     datetime = { .date = { .year = 2024, .month = 11, .day = 1, .weekday = Friday },
                                      .time = { .hour = 16, .minute = 8, .second = 32 } };

    will_return_datetime( datetime );
    performWaterscreenAction( &context );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenWeekendWithoutEnabledWeekends_standardModeState_doNothing ) };

    return cmocka_run_group_tests_name( "Standard mode state test ", tests, NULL, NULL );
}
