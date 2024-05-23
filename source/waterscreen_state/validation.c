/*
 * validation.c
 *
 *  Created on: 20 maj 2024
 *      Author: purch
 */
#include "validation.h"
#include "led_control.h"

#include <fsl_common.h>

void validateWaterscreenStatus(WaterscreenContext_t* context){
	switch(context->currentStateStatus){
	case kStatus_Success:{
		changeSuccessLed(OnDeviceState);
		changeErrorLed(OffDeviceState);
		break;
	}

	default:{
		changeSuccessLed(OffDeviceState);
		changeErrorLed(OnDeviceState);
	}

	}
}
