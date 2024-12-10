#include "ble_handler_task.h"

#include "logging_config.h"

#include <inttypes.h>
#include "task.h"

#define BLE_RECEIVE_BUFFER_CAPACITY 128
#define WORD_BUFFER_CAPACITY        64

typedef enum
{
    SmallNum,
    BigNum,
    Text
} DataType_t;

typedef uint32_t smallNum_t;
typedef uint64_t bigNum_t;
typedef uint8_t  typeInfo_t;
typedef uint8_t  textSize_t;

StreamBufferHandle_t g_rxBLEBuffer = NULL;

void bleReceiveTask( void * )
{
    g_rxBLEBuffer = xStreamBufferCreate( BLE_RECEIVE_BUFFER_CAPACITY, sizeof( typeInfo_t ) );
    if ( !g_rxBLEBuffer )
    {
        LogError( "Not enough heap memory to allocate BLE buffer!" );
        vTaskSuspend( NULL );
    }

    char       text[WORD_BUFFER_CAPACITY] = {};
    typeInfo_t typeInfo                   = 0;
    smallNum_t smallNum                   = 0;
    bigNum_t   bigNum                     = 0;
    for ( ;; )
    {
        // { 1B[typeInfo]xB[rest...] }
        xStreamBufferReceive( g_rxBLEBuffer, &typeInfo, sizeof( typeInfo_t ), portMAX_DELAY );

        switch ( typeInfo )
        {
        case SmallNum:
            {
                xStreamBufferSetTriggerLevel( g_rxBLEBuffer, sizeof( smallNum_t ) );
                xStreamBufferReceive( g_rxBLEBuffer, &smallNum, sizeof( smallNum_t ), portMAX_DELAY );
                LogDebug( "Got small: %" PRIu32, smallNum );
                break;
            }
        case BigNum:
            {
                xStreamBufferSetTriggerLevel( g_rxBLEBuffer, sizeof( bigNum_t ) );
                xStreamBufferReceive( g_rxBLEBuffer, &bigNum, sizeof( bigNum_t ), portMAX_DELAY );
                LogDebug( "Got big: %" PRIu64, bigNum );
                break;
            }
        case Text:
            {
                textSize_t textSize = 0;

                // Assume that word comes as { 1B[size]sizeB[text] }
                xStreamBufferReceive( g_rxBLEBuffer, &textSize, sizeof( textSize_t ), portMAX_DELAY );
                if ( 0 < textSize && textSize < WORD_BUFFER_CAPACITY )
                {
                    xStreamBufferSetTriggerLevel( g_rxBLEBuffer, textSize );
                    xStreamBufferReceive( g_rxBLEBuffer, text, textSize, portMAX_DELAY );

                    text[textSize] = 0; // EOL
                    LogDebug( "Got text: %s", text );
                }
                break;
            }
        default:
            LogDebug( "Unknown incoming data type" );
        }

        xStreamBufferSetTriggerLevel( g_rxBLEBuffer, sizeof( typeInfo_t ) );
    }
}
