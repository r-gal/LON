#ifndef ADCPHY_H
#define ADCPHY_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "SignalList.hpp"
#include "CommonDef.hpp"

extern void Error_Handler(void);

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

extern ADC_HandleTypeDef hadc3;

extern DTS_HandleTypeDef hdts;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void DMA1_Stream0_IRQHandler(void);
void BDMA_Channel0_IRQHandler(void);

void MX_ADC1_Init(void);
void MX_ADC3_Init(void);

void MX_DMA_Init(void);
void MX_BDMA_Init(void);

void VrefInit(void);



/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_DTS_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */