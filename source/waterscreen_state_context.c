#include "waterscreen_state_context.h"

waterscreenState_t g_waterscreenState = {.demoMode = false, .isPresenting = false, .valveOpenStateCounter = 0}; // old state concept

WaterscreenContext_t g_waterscreenContext = { .waterscreenStateHandler = idleState};

void changeWaterscreenState(waterscreenStateFunction_t newState){
	g_waterscreenContext.waterscreenStateHandler = newState;
}

void performWaterscreenAction(){
	g_waterscreenContext.waterscreenStateHandler();
}

