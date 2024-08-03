#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "pictures.h"

#include "power_control.h"
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

static void givenPicture_presentationState_printBottomUp( void **state )
{
    WaterscreenContext_t context = { .waterscreenStateHandler = presentationState,
                                     .picture                 = &picture,
                                     .demoLoopCount           = 0,
                                     .valveOpenCounter        = picture.rowCount - 1,
                                     .currentStateStatus      = Success };
    for ( int8_t i = picture.rowCount - 1; 0 <= i; --i )
    { // Print picture
        expect_value( sendDataToValves, *data, picture.dataBuffer[i] );
        will_return( shouldWaterPumpTrigger, false );
        will_return( shouldWaterAlaramTrigger, false );
        expect_value( manageWaterPump, state, OffDeviceState );
        will_return( sendDataToValves, Success );
        performWaterscreenAction( &context );
        assert_ptr_equal( context.waterscreenStateHandler, presentationState );
    }
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenPicture_presentationState_printBottomUp ),
    };

    return cmocka_run_group_tests_name( "Presentation State", tests, NULL, NULL );
}
