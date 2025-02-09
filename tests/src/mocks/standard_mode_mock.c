#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "standard_mode_state.h"

void initStandardModeConfig( const StandardModeConfig_t *cfg )
{
    assert_non_null( cfg );
}

void standardModeState( WaterscreenContext_t *context )
{
    assert_non_null( context );
}