/*
 * waterscreen_state_context.h
 *
 *  Created on: 16 maj 2024
 *      Author: purch
 */

#ifndef WATERSCREEN_STATE_CONTEXT_H_
#define WATERSCREEN_STATE_CONTEXT_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct {
	bool demoMode;
	bool isPresenting;
	uint32_t valveOpenStateCounter;
} waterscreenState_t;
extern waterscreenState_t g_waterscreenState;

typedef void (*waterscreenStateFunction_t)();
typedef struct {
	waterscreenStateFunction_t waterscreenStateHandler;
} WaterscreenContext_t;

extern WaterscreenContext_t g_waterscreenContext;

void changeWaterscreenState(waterscreenStateFunction_t newState);
void performWaterscreenAction();

#endif /* WATERSCREEN_STATE_CONTEXT_H_ */
