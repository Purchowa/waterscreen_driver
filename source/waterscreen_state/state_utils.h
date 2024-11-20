#ifndef STATE_UTILS_H_
#define STATE_UTILS_H_

#include "config/waterscreen_config.h"
#include "waterscreen_state_context.h"


const char *getStateName( const waterscreenStateFunction_t state );
const char *getModeName( const WaterscreenMode_t mode );

#endif /* STATE_UTILS_H_ */
