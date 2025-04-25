#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

#include "CommonDef.hpp"
#include "SignalList.hpp"
#include "LonInterfacePhy.hpp"


const uint16_t phyClkPin[] = PHY_CLK_PIN_LIST;
const uint16_t phyDataPin[]  = PHY_DATA_PIN_LIST;
const GPIO_TypeDef* phyClkGPIO[] = PHY_CLK_PORT_LIST;
const GPIO_TypeDef* phyDataGPIO[]  = PHY_DATA_PORT_LIST;

TIM_HandleTypeDef        htim3;

LonInterfaceTick_c lonInterfaceTick_sig; /* static signal */

LonInterfacePhy_c::LonInterfacePhy_c(uint8_t portNo_) : portNo(portNo_)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Pin = phyClkPin[portNo];
  HAL_GPIO_Init((GPIO_TypeDef*)phyClkGPIO[portNo], &GPIO_InitStruct);
  GPIO_InitStruct.Pin = phyDataPin[portNo];
  HAL_GPIO_Init((GPIO_TypeDef*)phyDataGPIO[portNo], &GPIO_InitStruct);

}

void TimerIcCallback( struct __TIM_HandleTypeDef *htim )
{
  lonInterfaceTick_sig.SendISR();
}

void LonInterfacePhy_c::InitInterfaceTick(void)
{

/*
  TimerHandle_t  timer = xTimerCreate("",pdMS_TO_TICKS(10),pdTRUE,( void * ) 0,vTimerCallback);
  xTimerStart(timer,0);
*/
  __HAL_RCC_TIM3_CLK_ENABLE();

  htim3.Instance = TIM3;

  /* Initialize TIMx peripheral as follow:
  + Period = [(TIM1CLK/1000) - 1]. to have a (1/1000) s time base.
  + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
  + ClockDivision = 0
  + Counter direction = Up
  */
  uint32_t timFreq = HAL_RCC_GetPCLK1Freq();
  htim3.Init.Period = LON_INTERFACE_FREQ;
  htim3.Init.Prescaler = timFreq/1000000;
  htim3.Init.ClockDivision = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;


  HAL_TIM_Base_Init(&htim3);
    htim3.PeriodElapsedCallback = TimerIcCallback;
  
  HAL_NVIC_EnableIRQ(TIM3_IRQn);
  HAL_NVIC_SetPriority(TIM3_IRQn, 14, 0U);
  HAL_TIM_Base_Start_IT(&htim3);

}

bool LonInterfacePhy_c::getClk(void)
{
  /* read IO input port */
  return HAL_GPIO_ReadPin((GPIO_TypeDef*)phyClkGPIO[portNo],phyClkPin[portNo]); 
}

 bool LonInterfacePhy_c::getDat(void)
{
  /* read IO input port */
  return HAL_GPIO_ReadPin( (GPIO_TypeDef*)phyDataGPIO[portNo],phyDataPin[portNo]); 
}


void LonInterfacePhy_c::setClkPhy(bool data)
{
  /* write IO input port */
  HAL_GPIO_WritePin((GPIO_TypeDef*)phyClkGPIO[portNo],phyClkPin[portNo],data?GPIO_PIN_SET:GPIO_PIN_RESET);
}

void LonInterfacePhy_c::setDatPhy(bool data)
{
  /* write IO input port */
  HAL_GPIO_WritePin( (GPIO_TypeDef*)phyDataGPIO[portNo],phyDataPin[portNo],data?GPIO_PIN_SET:GPIO_PIN_RESET);
}


#ifdef __cplusplus
 extern "C" {
#endif
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}


#ifdef __cplusplus
}
#endif



/*
void vTimerCallback( TimerHandle_t xTimer )
{
  lonInterfaceTick_sig.Send();
}

*/