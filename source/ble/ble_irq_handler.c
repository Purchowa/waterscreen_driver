#include "ble_handler_task.h"

#include "peripherals.h"


void FLEXCOMM7_IRQHandler()
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if ( kUSART_RxFifoNotEmptyFlag & USART_GetStatusFlags( BLE_USART_FC7_PERIPHERAL ) )
    {
        const uint8_t data = USART_ReadByte( BLE_USART_FC7_PERIPHERAL );

        xStreamBufferSendFromISR( g_rxBLEBuffer, &data, sizeof( data ), &xHigherPriorityTaskWoken );
    }
    portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
}
