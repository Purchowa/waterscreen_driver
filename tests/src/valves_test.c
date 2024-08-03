#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "pictures.h"

#include "spi_transfer.h"
#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

#include "utils/common_state_assert.h"


static const uint64_t pictKI[] = { 0b1000000000000000000000000000000000000000000000000000000000000000,
                                   0b0100000000000000000000000000000000000000000000000000000000000000,
                                   0b0010000000000000000000000000000000000000000000000000000000000000,
                                   0b0001000000000000000000000000000000000000000000000000000000000000 };

static const pictureData_t picture = { .dataBuffer = pictKI, .rowCount = 4 };

void assignPicture( const pictureData_t **dest ) { *dest = &picture; }

static void givenAnyState_closeValveSubState_turnValvesOffRemainTheSameState( void **state )
{
    WaterscreenContext_t context = { .waterscreenStateHandler = NULL,
                                     .picture                 = NULL,
                                     .demoLoopCount           = 0,
                                     .valveOpenCounter        = 0,
                                     .currentStateStatus      = Success };

    assertClosedValves();
    closeValvesSubState( &context );

    assert_null( context.waterscreenStateHandler ); // State handler should remain the same
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenAnyState_closeValveSubState_turnValvesOffRemainTheSameState ) };

    return cmocka_run_group_tests_name( "Water valves", tests, NULL, NULL );
}
