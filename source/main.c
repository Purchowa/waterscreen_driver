#include "config/valves_spi_cfg.h"
#include "config/oled_i2c_cfg.h"

#include "rtos_tasks/rtos_tasks.h"
#include "datetime/rtc_provider.h"

#include "external/oled/oled.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_debug_console.h"
#include "fsl_spi.h"
#include "fsl_i2c.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"


int main()
{
    // Attach clocks to peripherals-
    CLOCK_AttachClk( VALVES_SPI_CLOCK );
    CLOCK_AttachClk( OLED_I2C_CLOCK );

    /*reset FLEXCOMM's*/
    RESET_PeripheralReset( VALVES_SPI_RESET );
    RESET_PeripheralReset( OLED_I2C_RESET );
    // --

    CLOCK_AttachClk( BOARD_DEBUG_UART_CLK_ATTACH );
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    SystemCoreClockUpdate();

    initRTC();

    // --- Peripherals
    // ---- OLED FC1
    i2c_master_config_t oledConfig = {};
    I2C_MasterGetDefaultConfig( &oledConfig );
    oledConfig.baudRate_Bps = OLED_I2C_BAUD_RATE;
    oledConfig.enableMaster = true;
    oledConfig.timeout_Ms   = OLED_I2C_TIMEOUT_MS;
    I2C_MasterInit( OLED_I2C, &oledConfig, OLED_I2C_SRC_FREQ );
    OLED_Init( OLED_I2C );

    // ---- Valves FC3
    spi_master_config_t valvesMasterConfig = { 0 };
    SPI_MasterGetDefaultConfig( &valvesMasterConfig );
    valvesMasterConfig.sselNum                   = (spi_ssel_t)VALVES_SPI_SSEL;
    valvesMasterConfig.sselPol                   = (spi_spol_t)VALVES_SPI_SPOL;
    valvesMasterConfig.delayConfig.transferDelay = 1;
    valvesMasterConfig.direction                 = kSPI_LsbFirst; // Least significant bit.
    valvesMasterConfig.baudRate_Bps              = VALVE_SPI_BAUD_RATE;
    SPI_MasterInit( VALVES_SPI_MASTER, &valvesMasterConfig, VALVES_SPI_MASTER_SRC_FREQ );
    // !--


    if ( xTaskCreate( waterscreenActionTask, "WaterScreenActionTask", WATERSCREEN_ACTION_TASK_STACK_SIZE, NULL,
                      WATERSCREEN_ACTION_TASK_PRIORITY, NULL ) != pdPASS )
    {
        PRINTF( "[RTOS]-Task: WaterScreenAction task creation failed!\r\n" );
    }

    if ( xTaskCreate( hmiTask, "HmiTask", HMI_TASK_STACK_SIZE, NULL, HMI_TASK_PRIORITY, NULL ) != pdPASS )
    {
        PRINTF( "[RTOS]-Task: HMI task creation failed!.\r\n" );
    }

    if ( xTaskCreate( wifiTask, "WiFiTask", WIFI_TASK_STACK_SIZE, NULL, WIFI_TASK_PRIORITY, NULL ) != pdPASS )
    {
        PRINTF( "[RTOS]-Task: WIFI task creation failed!.\r\n" );
    }

    vTaskStartScheduler();
    for ( ;; )
    {
        PRINTF( "Task scheduler hasn't started or was stopped!" );
    }

    return 0;
}
