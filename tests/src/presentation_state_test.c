#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "pictures.h"

#include "power_control.h"
#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

#include "utils/common_state_assert.h"

static const uint64_t pictureSample[] = { 0b1000000000000000000000000000000000000000000000000000000000000000,
                                          0b0100000000000000000000000000000000000000000000000000000000000000,
                                          0b0010000000000000000000000000000000000000000000000000000000000000,
                                          0b0001000000000000000000000000000000000000000000000000000000000000 };

static const pictureDataView_t picture = { .data = pictureSample, .size = 4 };

static void givenPicture_presentationState_printBottomUp()
{
    will_return( getEachPictureView, &picture );
    const pictureDataView_t *mockedPicture = getEachPictureView();

    WaterscreenContext_t context = { .waterscreenStateHandler = presentationState,
                                     .pictureView             = mockedPicture,
                                     .valveOpenCounter        = mockedPicture->size - 1,
                                     .currentStateStatus      = SuccessSPI };
    for ( int8_t i = mockedPicture->size - 1; 0 <= i; --i )
    { // Print picture
        expect_value( sendDataToValves, *data, mockedPicture->data[i] );
        will_return( shouldWaterPumpTrigger, false );
        will_return( shouldWaterAlaramTrigger, false );
        expect_value( manageWaterPump, state, OffDeviceState );
        will_return( sendDataToValves, SuccessSPI );
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
