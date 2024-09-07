#include <stdarg.h>
#include <setjmp.h>
#include <stddef.h>
#include <cmocka.h>

#include "power_control.h"
#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

#include "utils/common_state_assert.h"

void givenAlarmOnPumpOff_checkSensorsSubState_transitionToLowWaterStateWithClosedValves( void **state )
{
    WaterscreenContext_t context = {
        .waterscreenStateHandler = NULL, .picture = NULL, .valveOpenCounter = 0, .currentStateStatus = Success };

    assertClosedValves();
    will_return( shouldWaterAlaramTrigger, true );
    will_return( shouldWaterPumpTrigger, false );
    expect_value( manageWaterPump, state, OffDeviceState );
    checkSensorsSubState( &context );

    assert_ptr_equal( context.waterscreenStateHandler, lowWaterState );
}

void givenAlarmOnPumpOn_checkSensorsSubState_transitionToLowWaterStateWithClosedValves( void **state )
{
    WaterscreenContext_t context = {
        .waterscreenStateHandler = NULL, .picture = NULL, .valveOpenCounter = 0, .currentStateStatus = Success };

    assertClosedValves();
    will_return( shouldWaterAlaramTrigger, true );
    will_return( shouldWaterPumpTrigger, true );
    expect_value( manageWaterPump, state, OffDeviceState );
    checkSensorsSubState( &context );

    assert_ptr_equal( context.waterscreenStateHandler, lowWaterState );
}

void givenAlarmOffPumpOff_checkSensorsSubState_remainInitialStateWithOpenValves( void **state )
{
    WaterscreenContext_t context = {
        .waterscreenStateHandler = NULL, .picture = NULL, .valveOpenCounter = 0, .currentStateStatus = Success };

    will_return( shouldWaterAlaramTrigger, false );
    will_return( shouldWaterPumpTrigger, false );
    expect_value( manageWaterPump, state, OffDeviceState );
    checkSensorsSubState( &context );

    assert_null( context.waterscreenStateHandler );
}

void givenAlarmOffPumpOn_checkSensorsSubState_remainInitialStateWithOpenValves( void **state )
{
    WaterscreenContext_t context = {
        .waterscreenStateHandler = NULL, .picture = NULL, .valveOpenCounter = 0, .currentStateStatus = Success };

    will_return( shouldWaterAlaramTrigger, false );
    will_return( shouldWaterPumpTrigger, true );
    expect_value( manageWaterPump, state, OnDeviceState );
    checkSensorsSubState( &context );

    assert_null( context.waterscreenStateHandler );
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test( givenAlarmOnPumpOff_checkSensorsSubState_transitionToLowWaterStateWithClosedValves ),
        cmocka_unit_test( givenAlarmOnPumpOn_checkSensorsSubState_transitionToLowWaterStateWithClosedValves ),
        cmocka_unit_test( givenAlarmOffPumpOff_checkSensorsSubState_remainInitialStateWithOpenValves ),
        cmocka_unit_test( givenAlarmOffPumpOn_checkSensorsSubState_remainInitialStateWithOpenValves ) };

    return cmocka_run_group_tests_name( "Presentation State", tests, NULL, NULL );
}
