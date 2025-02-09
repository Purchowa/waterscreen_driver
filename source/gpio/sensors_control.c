#ifndef SENSORS_CONTROL_C_
#define SENSORS_CONTROL_C_

#include <fsl_gpio.h>
#include <pin_mux.h>
#include "logging.h"

#define OPTIMAL_STATE 1
#define LOW_STATE     0


bool shouldWaterPumpTrigger()
{
    static uint32_t prevUpper         = LOW_STATE;
    static uint32_t prevBottom        = OPTIMAL_STATE;
    static bool     shouldPumpTrigger = false;

    const uint32_t upperBuoyState =
        GPIO_PinRead( BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_1_GPIO, BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_1_PORT,
                      BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_1_PIN );
    const uint32_t bottomBuoyState =
        GPIO_PinRead( BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_2_GPIO, BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_2_PORT,
                      BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_2_PIN );

    if ( prevBottom == OPTIMAL_STATE && bottomBuoyState == LOW_STATE )
        shouldPumpTrigger = true;
    if ( prevUpper == LOW_STATE && upperBuoyState == OPTIMAL_STATE )
        shouldPumpTrigger = false;

    if ( prevUpper != upperBuoyState || prevBottom != bottomBuoyState )
    {
        LogDebug( "1. Upper buoy sensor %s", upperBuoyState == LOW_STATE ? "low" : "optimal" );
        LogDebug( "2. Bottom buoy sensor %s", bottomBuoyState == LOW_STATE ? "low" : "optimal" );
        prevUpper  = upperBuoyState;
        prevBottom = bottomBuoyState;
    }

    return shouldPumpTrigger;
}

bool shouldWaterAlarmTrigger()
{
    return GPIO_PinRead( BOARD_INITGPIOPINS_BOTTOM_WATER_SENSOR_GPIO, BOARD_INITGPIOPINS_BOTTOM_WATER_SENSOR_PORT,
                         BOARD_INITGPIOPINS_BOTTOM_WATER_SENSOR_PIN ) == LOW_STATE;
}

#endif /* SENSORS_CONTROL_C_ */
