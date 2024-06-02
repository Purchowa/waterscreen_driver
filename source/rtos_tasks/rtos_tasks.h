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

#ifndef MSEC_TO_TICK
#define MSEC_TO_TICK(msec) ((uint32_t)(msec) * (uint32_t)configTICK_RATE_HZ / 1000uL)
#endif

#define HMI_TASK_PRIORITY (configTIMER_TASK_PRIORITY - 2) // Software Time must have the highest priority.
#define HMI_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 100)

#define WIFI_TASK_PRIORITY (configTIMER_TASK_PRIORITY - 1)
#define WIFI_TASK_STACK_SIZE (configMINIMAL_STACK_SIZE + 300)

#define SW_TIMER_PERIOD_MS MSEC_TO_TICK(14)

void hmiTask(void *params);
void wifiTask(void *params);
void swMainTimerCallback(TimerHandle_t xTimer);


#endif /* RTOS_TASKS_H_ */
