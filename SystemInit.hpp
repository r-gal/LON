#ifndef SYSINIT_H
#define SYSINIT_H

#include "CommonDef.hpp"

#ifdef STM32H725xx
#define TIM_1_IRQ TIM1_UP_IRQn
#endif

#ifdef STM32F446xx
#define TIM_1_IRQ TIM1_UP_TIM10_IRQn
#endif

void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
void MPU_Config(void);
void MX_IWDG1_Init(void);

#ifdef __cplusplus
 extern "C" {
#endif
  void TIM1_UP_IRQHandler(void);
  
  void Tim1PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#ifdef __cplusplus
}
#endif

#endif