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
#include "fsl_common.h"

#include "waterscreen_state_context.h"
#include "waterscreen_states.h"
#include "pictures.h"
#include "validation.h"

#if RUN_UNIT_TESTS

#include "test_main.h"

#endif

/*** RTOS WiFi Task configuration ***/

// Task priority
#define WIFI_TASK_PRIORITY (configTIMER_TASK_PRIORITY - 1) // Software Time must have the highest priority.

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

/* --- */

/*** Board buttons ***/
static bool isS3ButtonPressed(){
	return (GPIO_PinRead(BOARD_INITBUTTONSPINS_S3_GPIO, BOARD_INITBUTTONSPINS_S3_PORT, BOARD_INITBUTTONSPINS_S3_PIN) == 0);
}

static bool isS2ButtonPressed(){
	return (GPIO_PinRead(BOARD_INITBUTTONSPINS_S2_GPIO, BOARD_INITBUTTONSPINS_S2_PORT, BOARD_INITBUTTONSPINS_S2_PIN) == 0);
}
/* --- */

int main() {
	// Handle for timer.
	TimerHandle_t swTimerHandle = NULL;

	// SPI configuration definitions
    spi_master_config_t valvesMasterConfig = {0};

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


#if RUN_UNIT_TESTS
    runUnitTests();
#else
    const uint64_t initialPicture = 0;
    const pictureData_t initialPictureData = {.dataBuffer = &initialPicture, .rowCount = 0};

    WaterscreenContext_t waterscreenContext = {
    		.waterscreenStateHandler = idleState,
			.picture = &initialPictureData,
			.valveOpenCounter = 0,
			.currentStateStatus = kStatus_Success};

    if (xTaskCreate(mockWifiTask, "MockWifiTask", WIFI_TASK_STACK_SIZE, &waterscreenContext, WIFI_TASK_PRIORITY, NULL) !=
    		pdPASS)
    	{
    		PRINTF("WiFi task creation failed!.\r\n");
    		while (1)
    			;
    	}

    	// --- Timer for writing every row of water.
    	swTimerHandle = xTimerCreate("WaterLineBurstSWTimer", SW_TIMER_PERIOD_MS, pdTRUE, &waterscreenContext, swTimerWaterBurstCallback);

    	static const int ticksToWait = 0;
    	xTimerStart(swTimerHandle, ticksToWait);
    	vTaskStartScheduler();
#endif


	for (;;){
	}

	return 0;
}

static void mockWifiTask(void* context) {
	// PRINTF("Hello from WiFi task.\r\n");
  	for (;;) {
		if (isS3ButtonPressed()) {

			changeWaterscreenState((WaterscreenContext_t*) context, choosePictureState);
		}

		if (isS2ButtonPressed()){

			changeWaterscreenState((WaterscreenContext_t*) context, closeValvesState);
		}
	}

	vTaskSuspend(NULL); // Basically kill task.
}

static void swTimerWaterBurstCallback(TimerHandle_t xTimer) {
	WaterscreenContext_t* context = (WaterscreenContext_t*)pvTimerGetTimerID(xTimer); // Allowed in documentation

	performWaterscreenAction(context);
	validateWaterscreenStatus(context);

}
