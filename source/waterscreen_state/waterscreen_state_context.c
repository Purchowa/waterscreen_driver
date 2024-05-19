#include "waterscreen_state_context.h"
#include "waterscreen_states.h"

#include <fsl_common.h>
#include <fsl_gpio.h>
#include <pin_mux.h>

void changeWaterscreenState(WaterscreenContext_t* context, waterscreenStateFunction_t newState){
	context->waterscreenStateHandler = newState;
}

void performWaterscreenAction(WaterscreenContext_t* context){
	context->waterscreenStateHandler(context);
}

// Temporary solution for informing the user what is the current water screen status.
static void turnOnSuccessLed() {
	GPIO_PinWrite(BOARD_INITLEDSPINS_LED_BLUE_GPIO, BOARD_INITLEDSPINS_LED_BLUE_PORT, BOARD_INITLEDSPINS_LED_BLUE_PIN, 0);
}

static void turnOnErrorLed(){
	GPIO_PinWrite(BOARD_INITLEDSPINS_LED_RED_GPIO, BOARD_INITLEDSPINS_LED_RED_PORT, BOARD_INITLEDSPINS_LED_RED_PIN, 0);
}

void validateWaterscreenStatus(WaterscreenContext_t* context){
	switch(context->currentStateStatus){
	case kStatus_Success:
		turnOnSuccessLed();
		break;

	default:{
		turnOnErrorLed();
	}

	}
}
