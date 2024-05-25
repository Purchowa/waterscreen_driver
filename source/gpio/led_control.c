/*
 * led_control.c
 *
 *  Created on: 20 maj 2024
 *      Author: purch
 */
#include "led_control.h"

#include <fsl_gpio.h>
#include <pin_mux.h>
#include <board.h>

// Temporary solution for informing the user what is the current water screen status.
void changeSuccessLed(DeviceState_t state) {
	GPIO_PinWrite(BOARD_INITLEDSPINS_LED_BLUE_GPIO, BOARD_INITLEDSPINS_LED_BLUE_PORT, BOARD_INITLEDSPINS_LED_BLUE_PIN, state);
}

void changeErrorLed(DeviceState_t state){
	GPIO_PinWrite(BOARD_INITLEDSPINS_LED_RED_GPIO, BOARD_INITLEDSPINS_LED_RED_PORT, BOARD_INITLEDSPINS_LED_RED_PIN, state);
}
