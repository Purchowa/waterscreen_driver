#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

WaterscreenContext_t g_waterscreenContext = { .waterscreenStateHandler = idleState};

void changeWaterscreenState(waterscreenStateFunction_t newState){
	g_waterscreenContext.waterscreenStateHandler = newState;
}

void performWaterscreenAction(){
	g_waterscreenContext.waterscreenStateHandler();
}

