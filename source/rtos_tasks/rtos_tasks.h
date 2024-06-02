/*
 * hmi_task.h
 *
 *  Created on: 2 cze 2024
 *      Author: purch
 */

#ifndef RTOS_TASKS_H_
#define RTOS_TASKS_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#define HMI_TASK_PRIORITY (configTIMER_TASK_PRIORITY - 1) // Software Time must have the highest priority.
#define HMI_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 100)

#define SW_TIMER_PERIOD_MS (14 / portTICK_PERIOD_MS)

void hmiTask(void *params);
void swMainTimerCallback(TimerHandle_t xTimer);


#endif /* RTOS_TASKS_H_ */
