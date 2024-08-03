/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_debug_console.h"
#include "fsl_spi.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"

#include "valves_spi_cfg.h"
#include "rtos_tasks/rtos_tasks.h"

int main() {
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

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
	/* Init FSL debug console. */
	BOARD_InitDebugConsole();
#endif
	SystemCoreClockUpdate();


	// SPI Master configuration
	spi_master_config_t valvesMasterConfig = {0};
	SPI_MasterGetDefaultConfig(&valvesMasterConfig);
	uint32_t srcFreq            = VALVES_SPI_MASTER_CLK_FREQ;
	valvesMasterConfig.sselNum = (spi_ssel_t)VALVES_SPI_SSEL;
	valvesMasterConfig.sselPol = (spi_spol_t)VALVES_SPI_SPOL;
	valvesMasterConfig.delayConfig.transferDelay = 1;
	valvesMasterConfig.direction = kSPI_LsbFirst; // Least significant bit.
	valvesMasterConfig.baudRate_Bps = 12000000; // 12 MHz
	SPI_MasterInit(VALVES_SPI_MASTER, &valvesMasterConfig, srcFreq);
	// ---
	if (xTaskCreate(hmiTask, "hmiTask", HMI_TASK_STACK_SIZE, NULL, HMI_TASK_PRIORITY, NULL) !=
			pdPASS)
	{
		PRINTF("[RTOS]-Task: HMI task creation failed!.\r\n");
	}

	if (xTaskCreate(wifiTask, "wifiTask", WIFI_TASK_STACK_SIZE, NULL, WIFI_TASK_PRIORITY, NULL) !=
			pdPASS)
	{
		PRINTF("[RTOS]-Task: WIFI task creation failed!.\r\n");
	}


	TimerHandle_t swTimerHandle = xTimerCreate("WaterLineBurstSWTimer", SW_TIMER_PERIOD_MS, pdTRUE, NULL, swMainTimerCallback);
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
		PRINTF("Task scheduler hasn't started or was stopped!");
	}

	return 0;
}

