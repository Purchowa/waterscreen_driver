/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PERIPHERALS_H_
#define _PERIPHERALS_H_

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "fsl_common.h"
#include "fsl_i2c.h"
#include "fsl_spi.h"

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus */

/***********************************************************************************************************************
 * Definitions
 **********************************************************************************************************************/
/* Definitions for BOARD_InitPeripherals_cm33_core0 functional group */
/* BOARD_InitPeripherals_cm33_core0 defines for FLEXCOMM1 */
/* Definition of peripheral ID */
#define OLED_I2C_FC1_PERIPHERAL ((I2C_Type *)FLEXCOMM1)
/* Definition of the clock source frequency */
#define OLED_I2C_FC1_CLOCK_SOURCE 12000000UL
/* BOARD_InitPeripherals_cm33_core0 defines for FLEXCOMM3 */
/* Definition of peripheral ID */
#define VALVES_SPI_FC3_PERIPHERAL ((SPI_Type *)FLEXCOMM3)
/* Definition of the clock source frequency */
#define VALVES_SPI_FC3_CLOCK_SOURCE 12000000UL

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
extern const i2c_master_config_t OLED_I2C_FC1_config;
extern const spi_master_config_t VALVES_SPI_FC3_config;

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/

void BOARD_InitPeripherals_cm33_core0(void);

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void);

#if defined(__cplusplus)
}
#endif

#endif /* _PERIPHERALS_H_ */
