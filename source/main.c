#include "config/valves_spi_cfg.h"
#include "config/oled_i2c_cfg.h"

#include "rtos_tasks/rtos_tasks.h"
#include "ble/ble_handler_task.h"

#include "datetime/rtc_provider.h"
#include "neopixels/neopixel_defines.h"

#include "external/oled/oled.h"
#include "external/neopixel/neopixels.h"

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"

/* Freescale includes. */
#include "fsl_debug_console.h"
#include "fsl_spi.h"
#include "fsl_i2c.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "board.h"
#include "fsl_common.h"
#include "peripherals.h"


int main()
{
    CLOCK_AttachClk( BOARD_DEBUG_UART_CLK_ATTACH );
    /* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();

#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif
    SystemCoreClockUpdate();

    initRTC();
    OLED_Init( OLED_I2C );
    Neopixels_Init( NEOPIXELS_SPI_FC8_PERIPHERAL, TEST_NEOPIXEL_COUNT ); // TODO: Temporary neopixel amount for testing

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

    if ( xTaskCreate( bleReceiveTask, "BLE-Receive", BLE_RECEIVE_TASK_STACK_SIZE, NULL, BLE_RECEIVE_TASK_PRIORITY,
                      NULL ) != pdPASS )
    {
        PRINTF( "[RTOS]-Task: BLE-Receive task creation failed!.\r\n" );
    }

    vTaskStartScheduler();
    for ( ;; )
    {
        PRINTF( "[RTOS] Task scheduler hasn't started or was stopped!" );
    }

    return 0;
}
