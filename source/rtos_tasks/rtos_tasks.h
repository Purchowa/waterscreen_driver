#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#include "FreeRTOSConfig.h"


#define HMI_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 3 )
#define HMI_TASK_STACK_SIZE 512

#define WIFI_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 3 )
#define WIFI_TASK_STACK_SIZE 2048

#define WATERSCREEN_ACTION_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 2 )
#define WATERSCREEN_ACTION_TASK_STACK_SIZE 1024

void hmiTask( void *params );
void wifiTask( void *params );
void waterscreenActionTask( void *params );


#endif /* RTOS_TASKS_H_ */
