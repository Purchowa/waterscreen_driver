#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


#define HMI_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 3 ) // Software Timer must have the highest priority.
#define HMI_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE + 100 )

#define WIFI_TASK_PRIORITY   ( configTIMER_TASK_PRIORITY - 1 )
#define WIFI_TASK_STACK_SIZE ( configMINIMAL_STACK_SIZE + 512 )

#define SW_TIMER_PERIOD_MS pdMS_TO_TICKS( 14 )

void hmiTask( void *params );
void wifiTask( void *params );
void swMainTimerCallback( TimerHandle_t xTimer );


#endif /* RTOS_TASKS_H_ */
