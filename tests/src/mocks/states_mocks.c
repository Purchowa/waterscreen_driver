#include <stdarg.h>
#include <setjmp.h>
#include <cmocka.h>

#include "waterscreen_states.h"
#include "standard_mode_state.h"
#include "realtime_mode.h"

void demoModeState( WaterscreenContext_t *context )
{
    assert_non_null( context );
}
void idleState( WaterscreenContext_t *context )
{
    assert_non_null( context );
}
void standardModeState( WaterscreenContext_t *context )
{
    assert_non_null( context );
}

void setRtPicturePresentationRepeatCount( uint8_t n )
{
    ;
}

void realtimeModeState( WaterscreenContext_t *context )
{
    assert_non_null( context );
}
