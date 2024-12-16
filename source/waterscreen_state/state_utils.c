#include "state_utils.h"

#include "standard_mode_state.h"
#include "waterscreen_states.h"

#define WATERSCREEN_STATE_COUNT 5
#define UNKNOWN_STATE           "unknown"


typedef struct
{
    waterscreenStateFunction_t key;
    const char                *value;
} stateNamePair_t;

const char *getStateName( const waterscreenStateFunction_t state )
{
    static const stateNamePair_t statesMap[WATERSCREEN_STATE_COUNT] = {
        { .key = demoModeState, .value = "demonstration" },
        { presentationState, "presentation" },
        { idleState, "idle" },
        { lowWaterState, "low water" },
        { standardModeState, "standard" } };

    for ( uint8_t i = 0; i < WATERSCREEN_STATE_COUNT; ++i )
    {
        if ( state == statesMap[i].key )
            return statesMap[i].value;
    }

    return UNKNOWN_STATE;
}

const char *getModeName( const WaterscreenMode_t mode )
{
    switch ( mode )
    {
    case Mode_Standard:
        return "Standard";

    case Mode_Demo:
        return "Demo";

    case Mode_Service:
        return "Service";

    default:
        return UNKNOWN_STATE;
    }
}
