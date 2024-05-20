#include "waterscreen_states.h"
#include "waterscreen_state_context.h"
#include "pictures.h"
#include "power_control.h"
#include "spi_transfer/spi_transfer.h"

#define SINGLE_DEMO_PICTURE_COUNT 1

static inline uint16_t lastElementIndex(const pictureData_t* picture){
	return picture->rowCount - 1;
}

void choosePictureState(WaterscreenContext_t* context){
	static uint8_t pictureCounter = 0;

	assignPicture(context->picture);
	context->valveOpenCounter = lastElementIndex(context->picture);
	++pictureCounter;

	if (SINGLE_DEMO_PICTURE_COUNT < pictureCounter ){
		changeWaterscreenState(context, demoModeState);
	}
	else{
		changeWaterscreenState(context, closeValvesState);
	}

}

void demoModeState(WaterscreenContext_t* context) {
	if (context->valveOpenCounter <= 0){
		changeWaterscreenState(context, choosePictureState);
	}
	else{
		const status_t status = sendDataToValves(&context->picture->dataBuffer[context->valveOpenCounter--]); // Print picture in direction - bottom-up
		context->currentStateStatus = status;
	}
}

void closeValvesState(WaterscreenContext_t* context) {
	static const uint64_t closeValves = 0;

	const status_t status = sendDataToValves(&closeValves);
	context->currentStateStatus = status;

	manageValvePower(OffDeviceState);

	changeWaterscreenState(context, idleState);
}


void idleState(WaterscreenContext_t* ) {
	// Check for days, weeks, post to API, even the timer might be here lower.
}

