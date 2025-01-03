#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

#include "utils/common_state_assert.h"

static void givenAnyState_closeValveSubState_turnValvesOffRemainTheSameState( void **state )
{
    WaterscreenContext_t context = { .waterscreenStateHandler = presentationState };

    assertClosedValves();
    closeValvesSubState( &context );

    assert_ptr_equal( context.waterscreenStateHandler, presentationState ); // State handler should remain the same
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenAnyState_closeValveSubState_turnValvesOffRemainTheSameState ) };

    return cmocka_run_group_tests_name( "Water valves", tests, NULL, NULL );
}
