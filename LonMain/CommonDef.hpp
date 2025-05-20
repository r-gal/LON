#ifndef COMMON_DEF_H
#define COMMON_DEF_H
/*
#include "stm32h7xx.h"
#include "stm32h7xx_hal.h"*/

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

/* memory management definitions */

#ifdef STM32H725xx
#define MEM_USE_DTCM 1 /* used for processes, stacks and memory managers */
#define MEM_USE_RAM2 1 /* for ethernet */
#define RAM_SECTION_NAME ".AXI_RAM" 
#define DTCM_SECTION_NAME ".DTCM"
#define RAM2_SECTION_NAME ".RAM_D2"
#endif

#ifdef STM32F446xx
#define MEM_USE_DTCM 0 /* used for processes, stacks and memory managers */
#define MEM_USE_RAM2 0 /* for ethernet */
#define RAM_SECTION_NAME ".RAM1" 
#endif

/* configuration definitions */

#define LON_USE_ETHERNET 0
#define LON_USE_HTTP 0
#define LON_USE_TELNET 0
#define LON_USE_FTP 0

#define LON_USE_SDCARD 0
#define LON_USE_RNG 0
#define LON_BOOTUNIT 0
#if LON_BOOTUNIT == 0
#define VERSION_NAME "v1.1"
#endif

#define LON_USE_UART_TERMINAL 1

#define CONF_USE_TIME 0
#define CONF_USE_RUNTIME 0

#define DEBUG_PROCESS 1
#define DEBUG_SDCARD 0

#define LON_USE_PWR_MGMT 0
#define LON_USE_SENSORS_DATABASE 0

#define FAT_DEBUG 0

#define USE_COMMANDS 1

#define USE_WATCHDOG 1

#define DATA_0_Pin GPIO_PIN_1
#define DATA_0_GPIO_Port GPIOH
#define CLK_0_Pin GPIO_PIN_0
#define CLK_0_GPIO_Port GPIOH
#define DATA_1_Pin GPIO_PIN_9
#define DATA_1_GPIO_Port GPIOB
#define CLK_1_Pin GPIO_PIN_8
#define CLK_1_GPIO_Port GPIOB
#define DATA_2_Pin GPIO_PIN_5
#define DATA_2_GPIO_Port GPIOB
#define CLK_2_Pin GPIO_PIN_2
#define CLK_2_GPIO_Port GPIOD
#define DATA_3_Pin GPIO_PIN_12
#define DATA_3_GPIO_Port GPIOC
#define CLK_3_Pin GPIO_PIN_11
#define CLK_3_GPIO_Port GPIOC
#define DATA_4_Pin GPIO_PIN_10
#define DATA_4_GPIO_Port GPIOC
#define CLK_4_Pin GPIO_PIN_12
#define CLK_4_GPIO_Port GPIOA
#define DATA_5_Pin GPIO_PIN_11
#define DATA_5_GPIO_Port GPIOA
#define CLK_5_Pin GPIO_PIN_5
#define CLK_5_GPIO_Port GPIOA
#define DATA_6_Pin GPIO_PIN_9
#define DATA_6_GPIO_Port GPIOC
#define CLK_6_Pin GPIO_PIN_8
#define CLK_6_GPIO_Port GPIOC
#define DATA_7_Pin GPIO_PIN_7
#define DATA_7_GPIO_Port GPIOC
#define CLK_7_Pin GPIO_PIN_6
#define CLK_7_GPIO_Port GPIOC

/*
#define SPI4_INT_Pin GPIO_PIN_3
#define SPI4_INT_GPIO_Port GPIOE
#define V_main_Pin GPIO_PIN_6
#define V_main_GPIO_Port GPIOF
#define V_acu_Pin GPIO_PIN_7
#define V_acu_GPIO_Port GPIOF
#define C_ext_B_Pin GPIO_PIN_8
#define C_ext_B_GPIO_Port GPIOF
#define C_ext_BF9_Pin GPIO_PIN_9
#define C_ext_BF9_GPIO_Port GPIOF
#define C_main_Pin GPIO_PIN_10
#define C_main_GPIO_Port GPIOF
#define C_core_Pin GPIO_PIN_0
#define C_core_GPIO_Port GPIOC
#define C_port_4_Pin GPIO_PIN_0
#define C_port_4_GPIO_Port GPIOA
#define C_port_5_Pin GPIO_PIN_3
#define C_port_5_GPIO_Port GPIOA
#define C_port_6_Pin GPIO_PIN_4
#define C_port_6_GPIO_Port GPIOA
#define C_port_7_Pin GPIO_PIN_5
#define C_port_7_GPIO_Port GPIOA
#define C_port_0_Pin GPIO_PIN_6
#define C_port_0_GPIO_Port GPIOA
#define C_port_1_Pin GPIO_PIN_0
#define C_port_1_GPIO_Port GPIOB
#define C_port_2_Pin GPIO_PIN_1
#define C_port_2_GPIO_Port GPIOB
#define ETH_RESET_Pin GPIO_PIN_2
#define ETH_RESET_GPIO_Port GPIOB
#define C_port_3_Pin GPIO_PIN_11
#define C_port_3_GPIO_Port GPIOF

#define SPI2_U1_Pin GPIO_PIN_8
#define SPI2_U1_GPIO_Port GPIOD
#define SPI2_U2_Pin GPIO_PIN_9
#define SPI2_U2_GPIO_Port GPIOD
#define SPI2_INT_Pin GPIO_PIN_10
#define SPI2_INT_GPIO_Port GPIOD
#define USB_sense_Pin GPIO_PIN_6
#define USB_sense_GPIO_Port GPIOG
#define Usart_U2_Pin GPIO_PIN_7
#define Usart_U2_GPIO_Port GPIOG
#define Usart_U1_Pin GPIO_PIN_8
#define Usart_U1_GPIO_Port GPIOG
#define SD_DET_A_Pin GPIO_PIN_6
#define SD_DET_A_GPIO_Port GPIOC
#define CAN_U1_Pin GPIO_PIN_3
#define CAN_U1_GPIO_Port GPIOD

#define CAN_U2_Pin GPIO_PIN_13
#define CAN_U2_GPIO_Port GPIOG
#define SPI4_U2_Pin GPIO_PIN_14
#define SPI4_U2_GPIO_Port GPIOG
#define SPI4_U1_Pin GPIO_PIN_5
#define SPI4_U1_GPIO_Port GPIOB
#define Switch1_Pin GPIO_PIN_6
#define Switch1_GPIO_Port GPIOB
#define Switch2_Pin GPIO_PIN_7
#define Switch2_GPIO_Port GPIOB
#define Led1_Pin GPIO_PIN_8
#define Led1_GPIO_Port GPIOB
#define Led2_Pin GPIO_PIN_9
#define Led2_GPIO_Port GPIOB
#define PWR_ACU_LOAD_Pin GPIO_PIN_0
#define PWR_ACU_LOAD_GPIO_Port GPIOE
#define PWR_ACU_ENA_Pin GPIO_PIN_1
#define PWR_ACU_ENA_GPIO_Port GPIOE
*/

#ifdef __cplusplus
 extern "C" {
#endif

void Error_Handler(void);

#ifdef __cplusplus
}
#endif


#endif