#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

#include "power_control.h"
#include "spi_transfer.h"

void manageValvePower( DeviceState_t state )
{
    function_called();
}
void manageWaterPump( DeviceState_t state ) {}

status_t sendDataToValves( const uint64_t data )
{
    function_called();
}

bool shouldWaterPumpTrigger() {}

bool shouldWaterAlarmTrigger() {}


static void givenAnyState_closeValveSubState_turnValvesOffRemainTheSameState( void **state )
{
    WaterscreenContext_t context = { .waterscreenStateHandler = presentationState };

    expect_function_call( sendDataToValves );
    expect_function_call( manageValvePower );
    shutdownValves( &context );

    assert_ptr_equal( context.waterscreenStateHandler, presentationState ); // State handler should remain the same
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenAnyState_closeValveSubState_turnValvesOffRemainTheSameState ) };

    return cmocka_run_group_tests_name( "Water valves", tests, NULL, NULL );
}
