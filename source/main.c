#include "config/valves_spi_cfg.h"
#include "config/oled_i2c_cfg.h"

#include "lcd_screen/hmi_task.h"
#include "external_communication/wifi_task.h"
#include "waterscreen_state/presentation_state_machine_task.h"
#include "ble/ble_receiver.h"
#include "ble/ble_sender.h"

#include "datetime/rtc_provider.h"
#include "neopixels/neopixel_defines.h"

#include "external/lcd/lcd.h"
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
    LCD_Init( LCD_SPI_FC8_PERIPHERAL );
    Neopixels_Init( NEOPIXELS_SPI_FC1_PERIPHERAL, TEST_NEOPIXEL_COUNT ); // TODO: Temporary neopixel amount for testing

    if ( xTaskCreate( bleReceiverTask, "BLE-Receiver", BLE_RECEIVE_TASK_STACK_SIZE, NULL, BLE_RECEIVE_TASK_PRIORITY,
                      NULL ) != pdPASS )
    {
        PRINTF( "[RTOS]-Task: BLE-Receive task creation failed!.\r\n" );
    }

    if ( xTaskCreate( bleSenderTask, "BLE-Sender", BLE_SEND_TASK_STACK_SIZE, NULL, BLE_SEND_TASK_PRIORITY, NULL ) !=
         pdPASS )
    {
        PRINTF( "[RTOS]-Task: BLE-Send task creation failed!.\r\n" );
    }

    if ( xTaskCreate( presentationStateMachineTask, "PresentStateMachine", PRESENTATION_SM_TASK_STACK_SIZE, NULL,
                      PRESENTATION_SM_TASK_PRIORITY, NULL ) != pdPASS )
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
        PRINTF( "[RTOS] Task scheduler hasn't started or was stopped!" );
    }

    return 0;
}
