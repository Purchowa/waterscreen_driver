/*
 * hmi_task.c
 *
 *  Created on: 2 cze 2024
 *      Author: purch
 */

#include <rtos_tasks.h>
#include "button_control.h"
#include "waterscreen_states.h"
#include "waterscreen_state_context.h"
#include "validation.h"

#include "fsl_common.h"

static WaterscreenContext_t s_context = {
		.waterscreenStateHandler = idleState,
		.picture = NULL,
		.demoLoopCount = 1,
		.valveOpenCounter = 0,
		.currentStateStatus = kStatus_Fail
};

void hmiTask(void* params) {
  	for (;;) {
		if (isS3ButtonPressed() && s_context.waterscreenStateHandler == idleState) {
			changeWaterscreenState(&s_context, choosePictureState);
		}

		if (isS2ButtonPressed()){
			closeValvesSubState(&s_context);
			changeWaterscreenState(&s_context, idleState);
		}
	}
	vTaskSuspend(NULL); // Basically kill task.
}

void swMainTimerCallback(TimerHandle_t xTimer) {
	performWaterscreenAction(&s_context);
	validateWaterscreenStatus(&s_context);
}
