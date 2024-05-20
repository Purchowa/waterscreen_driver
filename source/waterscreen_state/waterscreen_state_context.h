/*
 * waterscreen_state_context.h
 *
 *  Created on: 16 maj 2024
 *      Author: purch
 */

#ifndef WATERSCREEN_STATE_CONTEXT_H_
#define WATERSCREEN_STATE_CONTEXT_H_

#define RUN_UNIT_TESTS 0

#include "pictures.h"

#include <stdbool.h>
#include <stdint.h>

typedef int32_t status_t;

struct WaterscreenContext;

typedef void (*waterscreenStateFunction_t)(struct WaterscreenContext*);

typedef struct WaterscreenContext{
	waterscreenStateFunction_t waterscreenStateHandler;
	const pictureData_t* picture;
	uint8_t demoLoopCount;
	int32_t valveOpenCounter;
	status_t currentStateStatus;
} WaterscreenContext_t;

void changeWaterscreenState(WaterscreenContext_t* context, waterscreenStateFunction_t newState);
void performWaterscreenAction(WaterscreenContext_t* context);

#endif /* WATERSCREEN_STATE_CONTEXT_H_ */
