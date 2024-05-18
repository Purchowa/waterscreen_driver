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

static void iteratePicture(uint8_t* valveOpenStateCounter, uint8_t* pictureIndex){
	*valveOpenStateCounter = 0;
	++(*pictureIndex);
}

void demoModeState() {
	static uint8_t valveOpenStateCounter = 0;
	static uint8_t pictureIndex = 0;

	sendDataToValves(&g_pictureData[pictureIndex].dataBuffer[valveOpenStateCounter++]);

	PRINTF("[%d]: valve burst!\r\n", valveOpenStateCounter);

	if (g_pictureData[pictureIndex].rowCount <= valveOpenStateCounter){
		iteratePicture(&valveOpenStateCounter, &pictureIndex); // One picture after another without delay.

		if (PICTURE_BUFFER_SIZE <= pictureIndex){
			pictureIndex = 0;
			changeWaterscreenState(closeValvesState);
		}
	}
}

void closeValvesState() {
	static const uint64_t closeValves = 0;

	sendDataToValves(&closeValves);

	changeWaterscreenState(idleState);
}


void idleState() {

}
