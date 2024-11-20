#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


#define HMI_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 2 )
#define HMI_TASK_STACK_SIZE 512

#define WIFI_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 2 )
#define WIFI_TASK_STACK_SIZE 2048

#define WATERSCREEN_ACTION_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 1 )
#define WATERSCREEN_ACTION_TASK_STACK_SIZE 1024


void hmiTask( void *params );
void wifiTask( void *params );
void waterscreenActionTask( void *params );


#endif /* RTOS_TASKS_H_ */
