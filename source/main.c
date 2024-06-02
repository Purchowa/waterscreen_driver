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


/*** RTOS WiFi Task configuration ***/

// Task priority
#define WIFI_TASK_PRIORITY (configTIMER_TASK_PRIORITY - 1) // Software Time must have the highest priority.

// Size in bytes I guess
#define WIFI_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 100)
static void mockWifiTask(void *pvParameters);
/* --- */


/*** RTOS Software (SW) Timer configuration ***/
#define SW_TIMER_PERIOD_MS (14 / portTICK_PERIOD_MS)
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

static WaterscreenContext_t s_context = {
		.waterscreenStateHandler = idleState,
		.picture = NULL,
		.demoLoopCount = 1,
		.valveOpenCounter = 0,
		.currentStateStatus = kStatus_Fail};

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

    if (xTaskCreate(mockWifiTask, "MockWifiTask", WIFI_TASK_STACK_SIZE, NULL, WIFI_TASK_PRIORITY, NULL) !=
    		pdPASS)
	{
		PRINTF("[RTOS]-Task: WiFi task creation failed!.\r\n");
	}


	swTimerHandle = xTimerCreate("WaterLineBurstSWTimer", SW_TIMER_PERIOD_MS, pdTRUE, NULL, swTimerWaterBurstCallback);
	if (swTimerHandle){
		if (xTimerStart(swTimerHandle, 0) != pdPASS){
			PRINTF("[RTOS]-Timer: start command could not be sent to the timer command queue\r\n");
		}
	}
	else{
		PRINTF("[RTOS]-Timer: insufficient FreeRTOS heap remaining to allocate the timer\r\n");
	}

	vTaskStartScheduler();


	for (;;){
		PRINTF("Critical error! - task scheduler hasn't started or was stopped!");
	}

	return 0;
}

static void mockWifiTask(void* params) {
	PRINTF("Hello from WiFi task.\r\n");
  	for (;;) {
		if (isS3ButtonPressed() && s_context.waterscreenStateHandler == idleState) {
			changeWaterscreenState(&s_context, choosePictureState);
		}

		if (isS2ButtonPressed()){
			closeValvesSubState(&s_context);
			changeWaterscreenState(&s_context, idleState);
		}
	}
	vTaskSuspend(NULL); // Basically kill task.

}

static void swTimerWaterBurstCallback(TimerHandle_t xTimer) {
	performWaterscreenAction(&s_context);
	validateWaterscreenStatus(&s_context);
}
