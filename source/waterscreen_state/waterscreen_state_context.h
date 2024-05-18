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

struct WaterscreenContext;

typedef void (*waterscreenStateFunction_t)(struct WaterscreenContext*);

typedef struct WaterscreenContext{
	waterscreenStateFunction_t waterscreenStateHandler;
	int data;
} WaterscreenContext_t;

void changeWaterscreenState(WaterscreenContext_t* context, waterscreenStateFunction_t newState);
void performWaterscreenAction(WaterscreenContext_t* context);

#endif /* WATERSCREEN_STATE_CONTEXT_H_ */
