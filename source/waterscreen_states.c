#include "waterscreen_states.h"
#include "pictures.h"
#include "valves_spi_cfg.h"

#include <fsl_debug_console.h>
#include <fsl_spi.h>

static void sendDataToValves(const uint64_t* data){
	static spi_transfer_t valvesTransfer = {.txData = NULL, .rxData = NULL, .dataSize = VALVE_BUFFER_SIZE, .configFlags = kSPI_FrameAssert};

	valvesTransfer.txData = (uint8_t*)data;

	SPI_MasterTransferBlocking(VALVES_SPI_MASTER, &valvesTransfer);
}

static void resetDemoModeState(uint8_t* valveOpenStateCounter) {
	*valveOpenStateCounter = 0;
}

void demoModeState() {
	static uint8_t valveOpenStateCounter = 0;

	sendDataToValves(&g_pictureData.dataBuffer[valveOpenStateCounter++]);

	PRINTF("[%d]: valve burst!\r\n", valveOpenStateCounter);

	if (g_pictureData.rowCount <= valveOpenStateCounter){
		resetDemoModeState(&valveOpenStateCounter);
		changeWaterscreenState(closeValvesState);
	}
}

void closeValvesState() {
	static const uint64_t closeValves = 0;

	sendDataToValves(&closeValves);

	changeWaterscreenState(idleState);
}


void idleState() {

}
