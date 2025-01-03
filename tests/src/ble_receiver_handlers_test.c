#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "receiver_handlers.h"
#include "waterscreen_states.h"
#include "standard_mode_state.h"

#include "waterscreen_config.h"
#include "waterscreen_state_context.h"


WaterscreenConfig_t              g_waterscreenConfig           = {};
WaterscreenContext_t             g_context                     = {};
const waterscreenStateFunction_t g_waterscreenModes[Mode_SIZE] = { standardModeState, demoModeState, idleState };

static void givenActiveRtModeAndCurrentModeStandard_handleBLERTModeEvents_changeModeToRTAndStateToIdle()
{
    const bool isRTModeActive = true;

    expect_value( forceChangeWaterscreenState, newState, idleState );
    handleBLERTModeEvent( isRTModeActive );

    assert_int_equal( g_waterscreenConfig.mode.current, Mode_RT );
    assert_int_equal( g_waterscreenConfig.mode.previous, Mode_Standard );
}

static void givenActiveRtModeAndCurrentModeRT_handleBLERTModeEvents_remainSameStateAndMode()
{
    const bool isRTModeActive        = true;
    g_waterscreenConfig.mode.current = Mode_RT;

    handleBLERTModeEvent( isRTModeActive );

    assert_int_equal( g_waterscreenConfig.mode.current, Mode_RT );
    assert_int_equal( g_waterscreenConfig.mode.previous, Mode_Standard );
}

static void givenNotActiveRtModeAndCurrentModeRT_handleBLERTModeEvents_changeModeToPrevious()
{
    const bool isRTModeActive        = false;
    g_waterscreenConfig.mode.current = Mode_RT;

    expect_value( forceChangeWaterscreenState, newState, standardModeState );
    handleBLERTModeEvent( isRTModeActive );

    assert_int_equal( g_waterscreenConfig.mode.current, Mode_Standard );
    assert_int_equal( g_waterscreenConfig.mode.previous, Mode_RT );
}
static void givenNotActiveRtModeAndCurrentModeStandard_handleBLERTModeEvents_remainSameStateAndMode()
{
    const bool isRTModeActive = false;

    handleBLERTModeEvent( isRTModeActive );

    assert_int_equal( g_waterscreenConfig.mode.current, Mode_Standard );
    assert_int_equal( g_waterscreenConfig.mode.previous, Mode_Standard );
}

static int setupGlobals()
{
    g_waterscreenConfig.mode.current  = Mode_Standard;
    g_waterscreenConfig.mode.previous = Mode_Standard;

    g_context.waterscreenStateHandler         = standardModeState;
    g_context.previousWaterscreenStateHandler = standardModeState;
    return 0;
}

int main()
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup(
            givenActiveRtModeAndCurrentModeStandard_handleBLERTModeEvents_changeModeToRTAndStateToIdle, setupGlobals ),
        cmocka_unit_test_setup( givenActiveRtModeAndCurrentModeRT_handleBLERTModeEvents_remainSameStateAndMode,
                                setupGlobals ),
        cmocka_unit_test_setup( givenNotActiveRtModeAndCurrentModeRT_handleBLERTModeEvents_changeModeToPrevious,
                                setupGlobals ),
        cmocka_unit_test_setup( givenNotActiveRtModeAndCurrentModeStandard_handleBLERTModeEvents_remainSameStateAndMode,
                                setupGlobals ),
    };

    return cmocka_run_group_tests_name( "BLE receiver handlers unit tests", tests, NULL, NULL );
}
