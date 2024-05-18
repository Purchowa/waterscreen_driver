/*
 * power_control.h
 *
 *  Created on: 18 maj 2024
 *      Author: purch
 */

#ifndef POWER_CONTROL_H_
#define POWER_CONTROL_H_

#include <stdbool.h>

// Logical 1 turns off, logical 0 turns on.
typedef enum {
	OnDeviceState,
	OffDeviceState
} DeviceState_t;

void manageValvePower(DeviceState_t state);

void manageWaterPump(DeviceState_t state);

#endif /* POWER_CONTROL_H_ */
