#include "waterscreen_states.h"
#include "waterscreen_state_context.h"
#include "pictures.h"
#include "valves_spi_cfg.h"

#include <fsl_debug_console.h>
#include <fsl_spi.h>

static void sendDataToValves(const uint64_t* data){
	static spi_transfer_t valvesTransfer = {.txData = NULL, .rxData = NULL, .dataSize = VALVE_BUFFER_SIZE, .configFlags = kSPI_FrameAssert};

	valvesTransfer.txData = (uint8_t*)data;

	SPI_MasterTransferBlocking(VALVES_SPI_MASTER, &valvesTransfer);
}

static uint8_t lastElementIndex(const uint8_t pictureIndex){
	return g_pictureData[pictureIndex].rowCount - 1;
}

void demoModeState() {
	static uint8_t pictureIndex = 0;
	static uint8_t valveOpenStateCounter = 0;
	valveOpenStateCounter = lastElementIndex(pictureIndex);

	// Print picture in direction - bottom-up
	sendDataToValves(&g_pictureData[pictureIndex].dataBuffer[valveOpenStateCounter--]);

	PRINTF("[%d]: valve burst!\r\n", valveOpenStateCounter);

	if (valveOpenStateCounter <= 0){
		pictureIndex++;

		if (PICTURE_BUFFER_SIZE <= pictureIndex){
			pictureIndex = 0;
			changeWaterscreenState(closeValvesState);
		}
		valveOpenStateCounter = lastElementIndex(pictureIndex);
	}
}

void closeValvesState() {
	static const uint64_t closeValves = 0;

	sendDataToValves(&closeValves);

	changeWaterscreenState(idleState);
}


void idleState() {

}
