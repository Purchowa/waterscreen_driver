#include "waterscreen_states.h"
#include "pictures.h"
#include "power_control.h"
#include "sensors_control.h"
#include "spi_transfer/spi_transfer.h"
#include "waterscreen_state_context.h"

#include <assert.h>

static inline uint16_t lastElementIndex(const pictureData_t *picture) {
	assert(picture);
	return (picture->rowCount - 1);
}

void demoModeState(WaterscreenContext_t *context) {
	static uint8_t pictureCounter = 0;

	if (pictureCounter < context->demoLoopCount) {
		++pictureCounter;
		context->picture = getPicture();
		context->valveOpenCounter = lastElementIndex(context->picture);
		manageValvePower(OnDeviceState);
		changeWaterscreenState(context, presentationState);
	} else {
		pictureCounter = 0;
		closeValvesSubState(context);
		changeWaterscreenState(context, idleState);
	}
}

void checkSensorsSubState(WaterscreenContext_t *context) {
	const bool isWaterAlarmTriggered = shouldWaterAlaramTrigger();

	if (isWaterAlarmTriggered) {
		// Post waterAlaram to API.
		closeValvesSubState(context);
		context->currentStateStatus = 1;
		changeWaterscreenState(context, lowWaterState);
	}

	if (shouldWaterPumpTrigger() && !isWaterAlarmTriggered) {
		manageWaterPump(OnDeviceState);
	} else {
		manageWaterPump(OffDeviceState);
	}
}

void presentationState(WaterscreenContext_t *context) {
	checkSensorsSubState(context);

	if (context->valveOpenCounter < 0) {
		changeWaterscreenState(context,
				demoModeState); // TODO: go back to previous state.
		// Not necessarily demoMode
		// (might be standard or HTTP picture)
	} else {
		const status_t status = sendDataToValves(
				&context->picture->dataBuffer[context->valveOpenCounter--]);
		context->currentStateStatus = status;
	}
}

void closeValvesSubState(WaterscreenContext_t *context) {
	static const uint64_t closeValves = 0;

	const status_t status = sendDataToValves(&closeValves);
	context->currentStateStatus = status;

	manageValvePower(OffDeviceState);
}

void idleState(WaterscreenContext_t *context) {
	// Check for days, weeks, post to API, even the timer might be here lower.
	checkSensorsSubState(context);
}

void lowWaterState(WaterscreenContext_t *context) {
	manageWaterPump(OffDeviceState);
	if (!shouldWaterAlaramTrigger()) {
		changeWaterscreenState(context, idleState);
	}
}
