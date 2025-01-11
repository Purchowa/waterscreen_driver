#include "waterscreen_config.h"

#include "waterscreen_state/standard_mode_state.h"
#include "waterscreen_state/waterscreen_states.h"
#include "picture_management/picture_data.h"


WaterscreenConfig_t g_waterscreenConfig = { .mode = { .current = Mode_Standard, .previous = Mode_Standard },
                                            .standardModeConfig = { .isWorkingDuringWeekends = false,
                                                                    .workTimeInStandardMode  = 5,
                                                                    .idleTimeInStandardMode  = 5,
                                                                    .workRange = { .from = 7, .to = 18 } } };

// TODO: service mode is not implemented hence idle is used.
const waterscreenStateFunction_t g_waterscreenConfigAvailableModes[CONFIG_AVAILABLE_MODE_COUNT] = {
    standardModeState, demoModeState, idleState };
