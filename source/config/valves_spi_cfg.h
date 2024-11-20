#ifndef VALVES_SPI_CFG_H_
#define VALVES_SPI_CFG_H_

#define VALVES_SPI_MASTER          SPI3
#define VALVES_SPI_MASTER_IRQ      FLEXCOMM3_IRQn
#define VALVES_SPI_MASTER_CLK_SRC  kCLOCK_Flexcomm3
#define VALVES_SPI_MASTER_SRC_FREQ CLOCK_GetFlexCommClkFreq( 3U )
#define VALVES_SPI_SSEL            0                     // Slave Select
#define VALVES_SPI_SPOL            kSPI_SpolActiveAllLow // Slave Select polarity
#define VALVES_SPI_CLOCK           kFRO12M_to_FLEXCOMM3
#define VALVES_SPI_RESET           kFC3_RST_SHIFT_RSTn
#define VALVE_BUFFER_SIZE          8
#define VALVE_SPI_BAUD_RATE        12000000 // 12 MHz

#endif                                      /* VALVES_SPI_CFG_H_ */
