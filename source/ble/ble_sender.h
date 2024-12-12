#ifndef BLE_SEND_TASK_H_
#define BLE_SEND_TASK_H_

#include "common_types.h"

#include <FreeRTOS.h>
#include <stream_buffer.h>

#include <stdbool.h>


#define BLE_SEND_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 2 )
#define BLE_SEND_TASK_STACK_SIZE 256

void bleSenderTask( void *params );
bool bleWrite( const byte_t *data, const size_t size );

#endif /* BLE_SEND_TASK_H_ */
