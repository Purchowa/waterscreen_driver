#include "waterscreen_state_context.h"

#include "waterscreen_states.h"
#include "standard_mode_state.h"

#include <stddef.h>


WaterscreenContext_t g_context = { .waterscreenStateHandler         = standardModeState,
                                   .previousWaterscreenStateHandler = demoModeState,
                                   .pictureInfo                     = NULL,
                                   .valveOpenCounter                = 0,
                                   .stateStatus                     = 0,
                                   .stateDelay                      = SECOND_MS };
