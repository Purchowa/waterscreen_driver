
#ifndef WATERSCREEN_STATES_H_
#define WATERSCREEN_STATES_H_

#include "waterscreen_state_context.h"

void standardModeState( WaterscreenContext_t * );
void demoModeState( WaterscreenContext_t * );
void checkSensorsSubState( WaterscreenContext_t * );
void presentationState( WaterscreenContext_t * );
void closeValvesSubState( WaterscreenContext_t * );
void idleState( WaterscreenContext_t * );
void lowWaterState( WaterscreenContext_t * );

#endif /* WATERSCREEN_STATES_H_ */
