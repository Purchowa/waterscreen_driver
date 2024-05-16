/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_spi.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_power.h"

#include "waterscreen_state_context.h"
#include "waterscreen_states.h"



/*** RTOS WiFi Task configuration ***/

// Task priority
#define WIFI_TASK_PRIORITY (configMAX_PRIORITIES - 3)

// Size in bytes I guess
#define WIFI_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 100)

static void mockWifiTask(void *pvParameters);
/* --- */


/*** RTOS Software (SW) Timer configuration ***/

#define SW_TIMER_PERIOD_MS (100 / portTICK_PERIOD_MS)
#define NO_ID 0

static void swTimerWaterBurstCallback(TimerHandle_t xTimer);
/* --- */


/*** SPI configuration - writing to water valves ***/
// Flexcom 3 is on P20 (PMOD).

#include "valves_spi_cfg.h"

static uint8_t g_valveSrcPacketBuffer[VALVE_BUFFER_SIZE] = {0};
static uint8_t g_valveDestPacketBuffer[VALVE_BUFFER_SIZE] = {0};

// Testing SPI
static void test_initSourceValveBuffer(uint8_t* valveBuffer) {
	for (uint8_t i = 0; i < VALVE_BUFFER_SIZE; i++)
		g_valveSrcPacketBuffer[i] = i;
}

static bool test_valveSrcAndDestAreEqual(uint8_t* src, uint8_t* dest) {
	for (uint8_t i = 0; i < VALVE_BUFFER_SIZE; i++) {
		if (src[i] != dest[i])
			return false;
	}
	return true;
}

/* --- */

/*** Board buttons ***/
static bool isS3ButtonPressed(){
	return (GPIO_PinRead(BOARD_INITBUTTONSPINS_S3_GPIO, BOARD_INITBUTTONSPINS_S3_PORT, BOARD_INITBUTTONSPINS_S3_PIN) == 0);
}
/* --- */

int main() {
	// Handle for timer.
	TimerHandle_t swTimerHandle = NULL;

	// SPI configuration definitions
    spi_master_config_t valvesMasterConfig = {0};
    spi_transfer_t valvesTransfer = {0};

    // SPI 3 - Flexcom 7 clock configuration --
    /*attach clock to SPI*/
    CLOCK_AttachClk(VALVES_SPI_CLOCK);

    /*reset FLEXCOMM for SPI*/
    RESET_PeripheralReset(VALVES_SPI_RESET);
    // --


	CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBUTTONsPins();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    SystemCoreClockUpdate();


    // SPI Master configuration
    SPI_MasterGetDefaultConfig(&valvesMasterConfig);
    uint32_t srcFreq            = VALVES_SPI_MASTER_CLK_FREQ;
    valvesMasterConfig.sselNum = (spi_ssel_t)VALVES_SPI_SSEL;
    valvesMasterConfig.sselPol = (spi_spol_t)VALVES_SPI_SPOL;
    valvesMasterConfig.delayConfig.transferDelay = 1;
    valvesMasterConfig.direction = kSPI_LsbFirst; // Least significant bit.
    valvesMasterConfig.baudRate_Bps = 12000000; // 12 MHz
    SPI_MasterInit(VALVES_SPI_MASTER, &valvesMasterConfig, srcFreq);
    // ---

    // SPI Transfer configuration
    valvesTransfer.txData      = g_valveSrcPacketBuffer;
    valvesTransfer.rxData      = g_valveDestPacketBuffer;
    valvesTransfer.dataSize    = VALVE_BUFFER_SIZE;
    valvesTransfer.configFlags = kSPI_FrameAssert;
    // ---


#if VALVES_SPI_TESTING
    {
		test_initSourceValveBuffer(g_valveSrcPacketBuffer);
		/*Start Transfer*/
		const status_t transferStatus = SPI_MasterTransferBlocking(VALVES_SPI_MASTER, &valvesTransfer);
		const bool areBuffersEqual = test_valveSrcAndDestAreEqual(g_valveSrcPacketBuffer, g_valveDestPacketBuffer);
		PRINTF(areBuffersEqual ? "SUCCESS: SPI transfer\r\n" : "FAIL: SPI transfer");
		PRINTF(" status: %d.\r\n", transferStatus);
    }
#endif

	if (xTaskCreate(mockWifiTask, "MockWifiTask", WIFI_TASK_STACK_SIZE, NULL, WIFI_TASK_PRIORITY, NULL) !=
		pdPASS)
	{
		PRINTF("WiFi task creation failed!.\r\n");
		while (1)
			;
	}

	// --- Timer for writing every row of water.
	swTimerHandle = xTimerCreate("WaterLineBurstSWTimer", SW_TIMER_PERIOD_MS, pdTRUE, NO_ID, swTimerWaterBurstCallback);

	static const int ticksToWait = 0;
	xTimerStart(swTimerHandle, ticksToWait);
	vTaskStartScheduler();
	for (;;){
	}


	return 0;
}

static void mockWifiTask(void* pvParameters) {
	PRINTF("Hello from WiFi task.\r\n");
  	for (;;) {
		if (isS3ButtonPressed()) {
				// g_waterscreenState.demoMode = true; // stateHandler function
				changeWaterscreenState(demoModeState);
			}
	}

	vTaskSuspend(NULL); // Basically kill task.
}

static void swTimerWaterBurstCallback(TimerHandle_t xTimer) {
	performWaterscreenAction();
}

/*static void swTimerWaterBurstCallback(TimerHandle_t xTimer) {
	if (g_waterscreenState.demoMode){
		g_waterscreenState.isPresenting = true;

		if (g_waterscreenState.isPresenting) {
			++g_waterscreenState.valveOpenStateCounter;
			PRINTF("[%d]: valve burst!\r\n", g_waterscreenState.valveOpenStateCounter);

			// Demo
			// const status_t transferStatus = SPI_MasterTransferBlocking(VALVES_SPI_MASTER, &valvesTransfer);

			static const uint32_t mockWaterRowCount = 4;
			if (mockWaterRowCount <= g_waterscreenState.valveOpenStateCounter){
				turnOffWaterscreenDemo(&g_waterscreenState);
			}
		}

	}
}*/
