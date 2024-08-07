/*
 * led_control.h
 *
 *  Created on: 20 maj 2024
 *      Author: purch
 */

#ifndef LED_CONTROL_H_
#define LED_CONTROL_H_

#include "power_control.h"

// Temporary solution for informing the user what is the current water screen status.
void changeSuccessLed( DeviceState_t );

void changeErrorLed( DeviceState_t );

#endif /* LED_CONTROL_H_ */
