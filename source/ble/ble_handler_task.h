#ifndef BLE_HANDLER_TASK_H_
#define BLE_HANDLER_TASK_H_

#include "FreeRTOS.h"
#include "stream_buffer.h"


#define BLE_RECEIVE_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 2 )
#define BLE_RECEIVE_TASK_STACK_SIZE 256

extern StreamBufferHandle_t g_rxBLEBuffer;

void bleReceiveTask( void *params );

#endif /* BLE_HANDLER_TASK_H_ */
