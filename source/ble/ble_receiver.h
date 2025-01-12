#ifndef BLE_RECEIVE_TASK_H_
#define BLE_RECEIVE_TASK_H_

#include <FreeRTOS.h>
#include <stream_buffer.h>

#include <stdbool.h>


#define BLE_RECEIVE_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 2 )
#define BLE_RECEIVE_TASK_STACK_SIZE 512

extern StreamBufferHandle_t g_rxBLEBuffer;

void bleReceiverTask( void *params );
bool isLoggingActive();

#endif /* BLE_RECEIVE_TASK_H_ */
