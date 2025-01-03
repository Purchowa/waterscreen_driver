#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "waterscreen_state_context_handler.h"


void forceChangeWaterscreenState( WaterscreenContext_t *context, waterscreenStateFunction_t newState )
{
    assert_non_null( context );

    check_expected( newState );
}
