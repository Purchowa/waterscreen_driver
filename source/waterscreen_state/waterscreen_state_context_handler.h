#ifndef WATERSCREEN_STATE_CONTEXT_HANDLER_H_
#define WATERSCREEN_STATE_CONTEXT_HANDLER_H_

#include "waterscreen_state_context.h"


void changeWaterscreenState( WaterscreenContext_t *context, waterscreenStateFunction_t newState );
void forceChangeWaterscreenState( WaterscreenContext_t *context, waterscreenStateFunction_t newState );
void handleForcedStateChange( WaterscreenContext_t *context );
void performWaterscreenAction( WaterscreenContext_t *context );
void goBackToPreviousWaterscreenState( WaterscreenContext_t *context );

#endif /* WATERSCREEN_STATE_CONTEXT_HANDLER_H_ */
