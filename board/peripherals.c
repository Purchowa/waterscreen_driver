/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
!!GlobalInfo
product: Peripherals v13.0
processor: LPC55S69
package_id: LPC55S69JBD100
mcu_data: ksdk2_0
processor_version: 14.0.0
board: LPCXpresso55S69
functionalGroups:
- name: BOARD_InitPeripherals_cm33_core0
  UUID: 61d0725d-b300-49cb-9c66-b5edfbf8ffc1
  called_from_default_init: true
  selectedCore: cm33_core0
- name: BOARD_InitPeripherals_cm33_core1
  UUID: e2041cd4-ebb6-45a5-807f-e0c2dc047d48
  selectedCore: cm33_core1
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'system'
- type_id: 'system'
- global_system_definitions:
  - user_definitions: ''
  - user_includes: ''
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'uart_cmsis_common'
- type_id: 'uart_cmsis_common_9cb8e302497aa696fdbb5a4fd622c2a8'
- global_USART_CMSIS_common:
  - quick_selection: 'default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/

/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
component:
- type: 'gpio_adapter_common'
- type_id: 'gpio_adapter_common_57579b9ac814fe26bf95df0a384c36b6'
- global_gpio_adapter_common:
  - quick_selection: 'default'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/***********************************************************************************************************************
 * Included files
 **********************************************************************************************************************/
#include "peripherals.h"

/***********************************************************************************************************************
 * BOARD_InitPeripherals_cm33_core0 functional group
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * NVIC initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'NVIC'
- type: 'nvic'
- mode: 'general'
- custom_name_enabled: 'false'
- type_id: 'nvic_57b5eef3774cc60acaede6f5b8bddc67'
- functional_group: 'BOARD_InitPeripherals_cm33_core0'
- peripheral: 'NVIC'
- config_sets:
  - nvic:
    - interrupt_table:
      - 0: []
    - interrupts: []
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */

/* Empty initialization function (commented out)
static void NVIC_init(void) {
} */

/***********************************************************************************************************************
 * OLED_I2C_FC1 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'OLED_I2C_FC1'
- type: 'flexcomm_i2c'
- mode: 'I2C_Polling'
- custom_name_enabled: 'true'
- editing_lock: 'true'
- type_id: 'flexcomm_i2c_c8597948f61bd571ab263ea4330b9dd6'
- functional_group: 'BOARD_InitPeripherals_cm33_core0'
- peripheral: 'FLEXCOMM1'
- config_sets:
  - fsl_i2c:
    - i2c_mode: 'kI2C_Master'
    - clockSource: 'FXCOMFunctionClock'
    - clockSourceFreq: 'BOARD_BootClockPLL150M'
    - i2c_master_config:
      - enableMaster: 'true'
      - baudRate_Bps: '1000000'
      - enableTimeout: 'false'
      - timeout_Ms: '35'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const i2c_master_config_t OLED_I2C_FC1_config = {
  .enableMaster = true,
  .baudRate_Bps = 1000000UL,
  .enableTimeout = false,
  .timeout_Ms = 35U
};

static void OLED_I2C_FC1_init(void) {
  /* Initialization function */
  I2C_MasterInit(OLED_I2C_FC1_PERIPHERAL, &OLED_I2C_FC1_config, OLED_I2C_FC1_CLOCK_SOURCE);
}

/***********************************************************************************************************************
 * VALVES_SPI_FC3 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'VALVES_SPI_FC3'
- type: 'flexcomm_spi'
- mode: 'SPI_Polling'
- custom_name_enabled: 'true'
- editing_lock: 'true'
- type_id: 'flexcomm_spi_b6bbc4b9dd1d40a58bb9821123930d3a'
- functional_group: 'BOARD_InitPeripherals_cm33_core0'
- peripheral: 'FLEXCOMM3'
- config_sets:
  - fsl_spi:
    - spi_mode: 'kSPI_Master'
    - clockSource: 'FXCOMFunctionClock'
    - clockSourceFreq: 'BOARD_BootClockPLL150M'
    - spi_master_config:
      - enableLoopback: 'false'
      - enableMaster: 'true'
      - polarity: 'kSPI_ClockPolarityActiveHigh'
      - phase: 'kSPI_ClockPhaseFirstEdge'
      - direction: 'kSPI_LsbFirst'
      - baudRate_Bps: '12000000'
      - dataWidth: 'kSPI_Data8Bits'
      - sselNum: 'kSPI_Ssel0'
      - sselPol_set: ''
      - txWatermark: 'kSPI_TxFifo0'
      - rxWatermark: 'kSPI_RxFifo1'
      - delayConfig:
        - preDelay: '0'
        - postDelay: '0'
        - frameDelay: '0'
        - transferDelay: '1'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const spi_master_config_t VALVES_SPI_FC3_config = {
  .enableLoopback = false,
  .enableMaster = true,
  .polarity = kSPI_ClockPolarityActiveHigh,
  .phase = kSPI_ClockPhaseFirstEdge,
  .direction = kSPI_LsbFirst,
  .baudRate_Bps = 12000000UL,
  .dataWidth = kSPI_Data8Bits,
  .sselNum = kSPI_Ssel0,
  .sselPol = kSPI_SpolActiveAllLow,
  .txWatermark = kSPI_TxFifo0,
  .rxWatermark = kSPI_RxFifo1,
  .delayConfig = {
    .preDelay = 0U,
    .postDelay = 0U,
    .frameDelay = 0U,
    .transferDelay = 1U
  }
};

static void VALVES_SPI_FC3_init(void) {
  /* Initialization function */
  SPI_MasterInit(VALVES_SPI_FC3_PERIPHERAL, &VALVES_SPI_FC3_config, VALVES_SPI_FC3_CLOCK_SOURCE);
}

/***********************************************************************************************************************
 * NEOPIXELS_SPI_FC8 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'NEOPIXELS_SPI_FC8'
- type: 'flexcomm_spi'
- mode: 'SPI_Polling'
- custom_name_enabled: 'true'
- editing_lock: 'true'
- type_id: 'flexcomm_spi_b6bbc4b9dd1d40a58bb9821123930d3a'
- functional_group: 'BOARD_InitPeripherals_cm33_core0'
- peripheral: 'FLEXCOMM8'
- config_sets:
  - fsl_spi:
    - spi_mode: 'kSPI_Master'
    - clockSource: 'FXCOMFunctionClock'
    - clockSourceFreq: 'BOARD_BootClockPLL150M'
    - spi_master_config:
      - enableLoopback: 'false'
      - enableMaster: 'true'
      - polarity: 'kSPI_ClockPolarityActiveHigh'
      - phase: 'kSPI_ClockPhaseFirstEdge'
      - direction: 'kSPI_MsbFirst'
      - baudRate_Bps: '4000000'
      - dataWidth: 'kSPI_Data5Bits'
      - sselNum: 'kSPI_Ssel0'
      - sselPol_set: ''
      - txWatermark: 'kSPI_TxFifo0'
      - rxWatermark: 'kSPI_RxFifo1'
      - delayConfig:
        - preDelay: '0'
        - postDelay: '0'
        - frameDelay: '0'
        - transferDelay: '0'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const spi_master_config_t NEOPIXELS_SPI_FC8_config = {
  .enableLoopback = false,
  .enableMaster = true,
  .polarity = kSPI_ClockPolarityActiveHigh,
  .phase = kSPI_ClockPhaseFirstEdge,
  .direction = kSPI_MsbFirst,
  .baudRate_Bps = 4000000UL,
  .dataWidth = kSPI_Data5Bits,
  .sselNum = kSPI_Ssel0,
  .sselPol = kSPI_SpolActiveAllLow,
  .txWatermark = kSPI_TxFifo0,
  .rxWatermark = kSPI_RxFifo1,
  .delayConfig = {
    .preDelay = 0U,
    .postDelay = 0U,
    .frameDelay = 0U,
    .transferDelay = 0U
  }
};

static void NEOPIXELS_SPI_FC8_init(void) {
  /* Initialization function */
  SPI_MasterInit(NEOPIXELS_SPI_FC8_PERIPHERAL, &NEOPIXELS_SPI_FC8_config, NEOPIXELS_SPI_FC8_CLOCK_SOURCE);
}

/***********************************************************************************************************************
 * BLE_USART_FC7 initialization code
 **********************************************************************************************************************/
/* clang-format off */
/* TEXT BELOW IS USED AS SETTING FOR TOOLS *************************************
instance:
- name: 'BLE_USART_FC7'
- type: 'flexcomm_usart'
- mode: 'interrupts'
- custom_name_enabled: 'true'
- editing_lock: 'true'
- type_id: 'flexcomm_usart_af6b92423b2c384096121979731b5f28'
- functional_group: 'BOARD_InitPeripherals_cm33_core0'
- peripheral: 'FLEXCOMM7'
- config_sets:
  - interruptsCfg:
    - interrupts: 'kUSART_RxLevelInterruptEnable'
    - interrupt_vectors:
      - enable_rx_tx_irq: 'true'
      - interrupt_rx_tx:
        - IRQn: 'FLEXCOMM7_IRQn'
        - enable_interrrupt: 'enabled'
        - enable_priority: 'true'
        - priority: '3'
        - enable_custom_name: 'false'
  - usartConfig_t:
    - usartConfig:
      - clockSource: 'FXCOMFunctionClock'
      - clockSourceFreq: 'BOARD_BootClockPLL150M'
      - baudRate_Bps: '115200'
      - syncMode: 'kUSART_SyncModeDisabled'
      - parityMode: 'kUSART_ParityDisabled'
      - stopBitCount: 'kUSART_OneStopBit'
      - bitCountPerChar: 'kUSART_8BitsPerChar'
      - loopback: 'false'
      - txWatermark: 'kUSART_TxFifo0'
      - rxWatermark: 'kUSART_RxFifo1'
      - enableMatchAddress: 'false'
      - matchAddressConfig:
        - matchAddress: '0'
        - addressMode: 'automatic'
      - enableRx: 'true'
      - enableTx: 'true'
      - enableHardwareFlowControl: 'false'
      - enableRTS: 'false'
      - clockPolarity: 'kUSART_RxSampleOnFallingEdge'
      - enableContinuousSCLK: 'false'
 * BE CAREFUL MODIFYING THIS COMMENT - IT IS YAML SETTINGS FOR TOOLS **********/
/* clang-format on */
const usart_config_t BLE_USART_FC7_config = {
  .baudRate_Bps = 115200UL,
  .syncMode = kUSART_SyncModeDisabled,
  .parityMode = kUSART_ParityDisabled,
  .stopBitCount = kUSART_OneStopBit,
  .bitCountPerChar = kUSART_8BitsPerChar,
  .loopback = false,
  .txWatermark = kUSART_TxFifo0,
  .rxWatermark = kUSART_RxFifo1,
  .enableRx = true,
  .enableTx = true,
  .enableHardwareFlowControl = false,
  .enableMode32k = false,
  .clockPolarity = kUSART_RxSampleOnFallingEdge,
  .enableContinuousSCLK = false
};

static void BLE_USART_FC7_init(void) {
  USART_Init(BLE_USART_FC7_PERIPHERAL, &BLE_USART_FC7_config, BLE_USART_FC7_CLOCK_SOURCE);
  USART_EnableInterrupts(BLE_USART_FC7_PERIPHERAL, kUSART_RxLevelInterruptEnable);
  /* Interrupt vector FLEXCOMM7_IRQn priority settings in the NVIC. */
  NVIC_SetPriority(BLE_USART_FC7_FLEXCOMM_IRQN, BLE_USART_FC7_FLEXCOMM_IRQ_PRIORITY);
  /* Enable interrupt FLEXCOMM7_IRQn request in the NVIC. */
  EnableIRQ(BLE_USART_FC7_FLEXCOMM_IRQN);
}

/***********************************************************************************************************************
 * Initialization functions
 **********************************************************************************************************************/
void BOARD_InitPeripherals_cm33_core0(void)
{
  /* Initialize components */
  OLED_I2C_FC1_init();
  VALVES_SPI_FC3_init();
  NEOPIXELS_SPI_FC8_init();
  BLE_USART_FC7_init();
}

/***********************************************************************************************************************
 * BOARD_InitBootPeripherals function
 **********************************************************************************************************************/
void BOARD_InitBootPeripherals(void)
{
  BOARD_InitPeripherals_cm33_core0();
}
