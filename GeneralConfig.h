#ifndef GENERAL_CONFIG_H
#define GENERAL_CONFIG_H


/* memory management definitions */

#define RUN_FROM_RAM 0
#define RAM_VECTORS_START 0x00000000

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

#define LON_USE_ETHERNET 1


#define LON_USE_SDCARD 1
#define LON_USE_RNG 1
#define LON_BOOTUNIT 0
#if LON_BOOTUNIT == 0
#define VERSION_NAME "v1.2"
#endif

#define LON_USE_UART_TERMINAL 1

#define CONF_USE_TIME 1
#define CONF_USE_RUNTIME 1

#define DEBUG_PROCESS 1
#define DEBUG_SDCARD 0

#define LON_USE_PWR_MGMT 0
#define LON_USE_SENSORS_DATABASE 1

#define FAT_DEBUG 0

#define USE_COMMANDS 1

#define USE_WATCHDOG 1




#endif