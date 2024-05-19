#include "waterscreen_states.h"
#include "waterscreen_state_context.h"
#include "pictures.h"
#include "valves_spi_cfg.h"
#include "power_control.h"

#include <fsl_debug_console.h>

#if !RUN_UNIT_TESTS // What to do to run unit tests on this.
#include <fsl_spi.h>
#endif

static status_t sendDataToValves(const uint64_t* data){
	static spi_transfer_t valvesTransfer = {.txData = NULL, .rxData = NULL, .dataSize = VALVE_BUFFER_SIZE, .configFlags = kSPI_FrameAssert};

	valvesTransfer.txData = (uint8_t*)data;

	return SPI_MasterTransferBlocking(VALVES_SPI_MASTER, &valvesTransfer);
}

static uint8_t lastElementIndex(const uint8_t pictureIndex){
	return g_pictureData[pictureIndex].rowCount - 1;
}

void demoModeState(WaterscreenContext_t* context) {
	static uint8_t pictureIndex = -1;
	static uint8_t valveOpenStateCounter = 0;

	if (valveOpenStateCounter <= 0){
		++pictureIndex;

		if (PICTURE_BUFFER_SIZE <= pictureIndex){
			pictureIndex = 0;
			changeWaterscreenState(context, closeValvesState);
		}
		else{
			valveOpenStateCounter = lastElementIndex(pictureIndex);
		}
	}
	else{
		const status_t status = sendDataToValves(&g_pictureData[pictureIndex].dataBuffer[valveOpenStateCounter--]); // Print picture in direction - bottom-up
		context->currentStateStatus = status;
		// PRINTF("[%d]: valve burst!\r\n", valveOpenStateCounter);
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

