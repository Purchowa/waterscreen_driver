
#ifndef WATERSCREEN_STATES_H_
#define WATERSCREEN_STATES_H_

#include "waterscreen_state_context.h"

void demoModeState( WaterscreenContext_t * );
void checkSensorsSubState( WaterscreenContext_t * );
void presentationState( WaterscreenContext_t * );
void idleState( WaterscreenContext_t * );
void lowWaterState( WaterscreenContext_t * );
void serviceModeState( WaterscreenContext_t * );

/**
 * Function closes valves and turns off their power.
 */
void shutdownValves( WaterscreenContext_t * );

#endif /* WATERSCREEN_STATES_H_ */
