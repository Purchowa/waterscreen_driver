#include "ble_sender.h"

#include "logging.h"
#include "common_types.h"
#include "ble_receiver.h"

#include <task.h>
#include <fsl_usart.h>
#include <peripherals.h>
#include <stdint.h>

#define BLE_SEND_BUFFER_CAPACITY   256
#define SENDING_TASK_TRIGGER_LEVEL 1

#define STREAM_BUFFER_MAX_SEND_WAIT_MS 3

static StreamBufferHandle_t s_txBLEBuffer = NULL;
static byte_t               s_bytesBuffer[BLE_SEND_BUFFER_CAPACITY];

void bleSenderTask( void *params )
{
    s_txBLEBuffer = xStreamBufferCreate( BLE_SEND_BUFFER_CAPACITY, SENDING_TASK_TRIGGER_LEVEL );
    if ( !s_txBLEBuffer )
    {
        LogError( "Not enough heap memory to allocate BLE buffer!" );
        vTaskSuspend( NULL );
    }

    for ( ;; )
    {
        const size_t bytesRead =
            xStreamBufferReceive( s_txBLEBuffer, s_bytesBuffer, BLE_SEND_BUFFER_CAPACITY, portMAX_DELAY );

        USART_WriteBlocking( BLE_USART_FC7_PERIPHERAL, s_bytesBuffer, bytesRead );
    }
}

bool bleWrite( const byte_t *data, const size_t size )
{
    if ( !isClientConnected() || s_txBLEBuffer == NULL )
        return false;

    const size_t bytesSend =
        xStreamBufferSend( s_txBLEBuffer, data, size, pdMS_TO_TICKS( STREAM_BUFFER_MAX_SEND_WAIT_MS ) );

    return bytesSend == size;
}
