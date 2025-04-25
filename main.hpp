#ifndef MAIN_H
#define MAIN_H

#include "stm32h7xx_hal.h"
//#include "signal_main.hpp"
#include "CommonDef.hpp"

#ifdef __cplusplus
 extern "C" {
#endif
  void TIM1_UP_IRQHandler(void);
  
  void Tim10PeriodElapsedCallback(TIM_HandleTypeDef *htim);
#ifdef __cplusplus
}
#endif

#endif