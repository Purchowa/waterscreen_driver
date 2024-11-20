#ifndef OLED_I2C_CFG_H_
#define OLED_I2C_CFG_H_

// FLEXCOMM-1

#define OLED_I2C       ( (I2C_Type *)I2C1_BASE )
#define OLED_I2C_CLOCK kFRO12M_to_FLEXCOMM1
#define OLED_I2C_RESET kFC1_RST_SHIFT_RSTn

#define OLED_I2C_BAUD_RATE  1000000 // 1 MHz
#define OLED_I2C_TIMEOUT_MS 35
#define OLED_I2C_SRC_FREQ   CLOCK_GetFlexCommClkFreq( 1U )


#endif /* OLED_I2C_CFG_H_ */
