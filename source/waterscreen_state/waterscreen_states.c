#include "waterscreen_states.h"
#include "waterscreen_state_context.h"
#include "pictures.h"
#include "spi_transfer/spi_transfer.h"
#include "power_control.h"
#include "sensors_control.h"

static inline uint16_t lastElementIndex(const pictureData_t *picture)
{
	return picture->rowCount - 1;
}

void choosePictureState(WaterscreenContext_t *context)
{
	static uint8_t pictureCounter = 0;

	if (pictureCounter < context->demoLoopCount)
	{
		++pictureCounter;
		assignPicture(&context->picture);
		context->valveOpenCounter = lastElementIndex(context->picture);
		manageValvePower(OnDeviceState);
		changeWaterscreenState(context, demoModeState);
	}
	else
	{
		pictureCounter = 0;
		closeValvesSubState(context);
		changeWaterscreenState(context, idleState);
	}
}

void checkSensorsSubState(WaterscreenContext_t* context)
{
	const bool isWaterAlarmTriggered = shouldWaterAlaramTrigger();

	if (isWaterAlarmTriggered) {
		// Post waterAlaram to API.
		closeValvesSubState(context);
		context->currentStateStatus = 1;
		changeWaterscreenState(context, lowWaterState);
	}

	if (shouldWaterPumpTrigger() && !isWaterAlarmTriggered) {
		manageWaterPump(OnDeviceState);
	}
	else {
		manageWaterPump(OffDeviceState);
	}
}

void demoModeState(WaterscreenContext_t *context)
{
	checkSensorsSubState(context);

	if (context->valveOpenCounter < 0)
	{
		changeWaterscreenState(context, choosePictureState);
	}
	else
	{
		const status_t status = sendDataToValves(&context->picture->dataBuffer[context->valveOpenCounter--]); // Print picture in direction - bottom-up
		context->currentStateStatus = status;
	}
}

void closeValvesSubState(WaterscreenContext_t *context)
{
	static const uint64_t closeValves = 0;

	const status_t status = sendDataToValves(&closeValves);
	context->currentStateStatus = status;

	manageValvePower(OffDeviceState);
}

void idleState(WaterscreenContext_t* context)
{
	// Check for days, weeks, post to API, even the timer might be here lower.
}

void lowWaterState(WaterscreenContext_t* context)
{
	if (!shouldWaterAlaramTrigger()) {
		changeWaterscreenState(context, idleState);
	}
}
