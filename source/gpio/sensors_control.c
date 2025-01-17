#ifndef SENSORS_CONTROL_C_
#define SENSORS_CONTROL_C_

#include <fsl_gpio.h>
#include <pin_mux.h>

#define OPTIMAL_STATE 1
#define LOW_STATE     0


bool shouldWaterPumpTrigger()
{
    static bool s_shouldPumpTrigger = false;

    const uint32_t firstFloaterState =
        GPIO_PinRead( BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_1_GPIO, BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_1_PORT,
                      BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_1_PIN );
    const uint32_t secondFloaterState =
        GPIO_PinRead( BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_2_GPIO, BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_2_PORT,
                      BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_2_PIN );

    if ( firstFloaterState == LOW_STATE && secondFloaterState == LOW_STATE )
        s_shouldPumpTrigger = true;
    else if ( firstFloaterState == OPTIMAL_STATE && secondFloaterState == OPTIMAL_STATE )
        s_shouldPumpTrigger = false;

    return s_shouldPumpTrigger;
}

bool shouldWaterAlarmTrigger()
{
    return GPIO_PinRead( BOARD_INITGPIOPINS_BOTTOM_WATER_SENSOR_GPIO, BOARD_INITGPIOPINS_BOTTOM_WATER_SENSOR_PORT,
                         BOARD_INITGPIOPINS_BOTTOM_WATER_SENSOR_PIN ) == LOW_STATE;
}

#endif /* SENSORS_CONTROL_C_ */
