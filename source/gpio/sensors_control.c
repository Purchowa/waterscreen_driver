#ifndef SENSORS_CONTROL_C_
#define SENSORS_CONTROL_C_

#include <fsl_gpio.h>
#include <pin_mux.h>

bool shouldWaterPumpTrigger()
{
    return ( GPIO_PinRead( BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_1_GPIO, BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_1_PORT,
                           BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_1_PIN ) == 0 &&
             GPIO_PinRead( BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_2_GPIO, BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_2_PORT,
                           BOARD_INITGPIOPINS_UPPER_WATER_SENSOR_2_PIN ) == 0 );
}

bool shouldWaterAlaramTrigger()
{
    return GPIO_PinRead( BOARD_INITGPIOPINS_BOTTOM_WATER_SENSOR_GPIO, BOARD_INITGPIOPINS_BOTTOM_WATER_SENSOR_PORT,
                         BOARD_INITGPIOPINS_BOTTOM_WATER_SENSOR_PIN ) == 0;
}

#endif /* SENSORS_CONTROL_C_ */
