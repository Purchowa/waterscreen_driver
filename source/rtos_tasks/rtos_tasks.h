#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


#define HMI_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 3 )
#define HMI_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

#define WIFI_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 2 )
#define WIFI_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE + 512 )

#define WATERSCREEN_ACTION_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 1 )
#define WATERSCREEN_ACTION_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE + 512 )

void hmiTask( void *params );
void wifiTask( void *params );
void waterscreenActionTask( void *params );


#endif /* RTOS_TASKS_H_ */
