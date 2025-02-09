#include "waterscreen_config.h"

#include "waterscreen_state/standard_mode_state.h"
#include "waterscreen_state/waterscreen_states.h"


WaterscreenConfig_t g_waterscreenConfig = { .mode = { .current = Mode_Standard, .previous = Mode_Standard },
                                            .standardModeConfig = { .isWorkingDuringWeekends = false,
                                                                    .workTimeInStandardMode  = 1,
                                                                    .idleTimeInStandardMode  = 15,
                                                                    .workRange = { .from = 8, .to = 15 } } };

const waterscreenStateFunction_t g_waterscreenConfigAvailableModes[CONFIG_AVAILABLE_MODE_COUNT] = {
    standardModeState, demoModeState, serviceModeState };
