#include "waterscreen_config.h"

#include "waterscreen_state/standard_mode_state.h"
#include "waterscreen_state/waterscreen_states.h"


// TODO: service mode is not implemented hence idle is used.
const waterscreenStateFunction_t g_waterscreenModes[Mode_SIZE] = { standardModeState, demoModeState, idleState };
