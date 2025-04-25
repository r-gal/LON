#include <stdio.h>
#include <stdlib.h>

#pragma fullpath_file  off 

/* Scheduler includes. */
#include "FreeRTOS.h"

#include "task.h"

#include "main.hpp"

#include "CtrlProcess.hpp"
#include "HTTP_ServerProcess.hpp"
#include "TELNET_ServerProcess.hpp"
#include "FTP_ServerProcess.hpp"
#include "RunTimeStats.hpp"
#include "TcpProcess.hpp"

#include "PowerProcess.hpp"

#include "LonInterfaceProcess.hpp"
#include "LonTrafficProcess.hpp"
#include "LonSensorsDatabaseProcess.hpp"

#include "HeapManager.hpp"

#define configTOTAL_RAMD2_SIZE (30*1024)

uint8_t __attribute((section(".AXI_RAM"))) ucHeap[ configTOTAL_HEAP_SIZE ] __attribute__ ((aligned (4)));
uint8_t __attribute((section(".DTCM"))) ucCCMHeap[ configTOTAL_CCM_HEAP_SIZE ] __attribute__ ((aligned (4)));
uint8_t __attribute((section(".RAM_D2"))) ucD2Heap[ configTOTAL_RAMD2_SIZE ] __attribute__ ((aligned (4)));
HeapRegion_t xHeapRegions[] =
  {
  { ucHeap ,			configTOTAL_HEAP_SIZE },
/*  { (uint8_t*) SDRAM_START ,	SDRAM_SIZE },  DRAM1 */
  { NULL, 0 }
  };
HeapRegion_t xCCMHeapRegions[] =
  {
  { ucCCMHeap ,			configTOTAL_CCM_HEAP_SIZE },
/*  { (uint8_t*) SDRAM_START ,	SDRAM_SIZE },  DRAM1 */
  { NULL, 0 }
  };
HeapRegion_t xRAMD2HeapRegions[] =
  {
  { ucD2Heap ,			configTOTAL_RAMD2_SIZE },
/*  { (uint8_t*) SDRAM_START ,	SDRAM_SIZE },  DRAM1 */
  { NULL, 0 }
  };

uint16_t baseManagerSizes[] = {32,64,128,340,540,1600,4096,16384};
uint16_t ccmManagerSizes[] = {512,1024,2048,4096,8192,16384};
uint16_t ramD2ManagerSizes[] = {256,1600};

HeapManager_c __attribute((section(".DTCM"))) baseManager(8,baseManagerSizes,0);
HeapManager_c __attribute((section(".DTCM"))) ccmManager(6,ccmManagerSizes,1);
HeapManager_c __attribute((section(".DTCM"))) ramD2Manager(2,ramD2ManagerSizes,2);


#ifdef __cplusplus
 extern "C" {
#endif

void *pvPortMalloc( size_t xWantedSize )
{
  return baseManager.Malloc(xWantedSize,1024);
}

void vPortFree( void *pv )
{
  baseManager.Free(pv);
}

void * pvPortMallocStack( size_t xSize ) 
{
  return ccmManager.Malloc(xSize,1024);
}
void vPortFreeStack( void * pv )
{
  ccmManager.Free(pv);
}

#ifdef __cplusplus
}
#endif


void * operator new(size_t size) { 
  //return (pvPortMalloc(size));
  return baseManager.Malloc(size,0); 
}
void operator delete(void* ptr) {
  //vPortFree(wsk) ;

  uint8_t idx = HeapManager_c::GetManagerId(ptr);
  if(idx == 0)
  {
    baseManager.Free(ptr);
  }
  else if(idx == 1)
  {
    ccmManager.Free(ptr);
  }
  else if(idx == 2)
  {
    ramD2Manager.Free(ptr);
  }

  //baseManager.Free(ptr);
}
 void* operator new[](size_t size) {
  //return (pvPortMalloc(size));
  return baseManager.Malloc(size,0);
}
void operator delete[](void* ptr) {
  //vPortFree(ptr);
  uint8_t idx = HeapManager_c::GetManagerId(ptr);
  if(idx == 0)
  {
    baseManager.Free(ptr);
  }
  else if(idx == 1)
  {
    ccmManager.Free(ptr);
  }
  else if(idx == 2)
  {
    ramD2Manager.Free(ptr);
  }
  //baseManager.Free(ptr);
}

static void prvSetupHardware( void );
static void prvSetupHeap( void );

void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
void MPU_Config(void);
void MX_IWDG1_Init(void);



 
TIM_HandleTypeDef        htim1;
IWDG_HandleTypeDef hiwdg1;


/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void)
{

  SCB->VTOR = 0x00000000;

  __HAL_RCC_SYSCFG_CLK_ENABLE();

  #if USE_WATCHDOG == 1
  MX_IWDG1_Init();
  #endif

  //MPU_Config();
  
  HAL_Init();

  SystemClock_Config();
  PeriphCommonClock_Config();

  

/**********************/

/*
__HAL_RCC_GPIOB_CLK_ENABLE();

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  GPIO_InitStruct.Pin = Led1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(Led1_GPIO_Port, &GPIO_InitStruct);
*/

/**********************/


  prvSetupHardware();
  prvSetupHeap();
/*
  SCB_DisableICache();
  SCB_EnableDCache ();
  SCB_DisableDCache ();*/
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();

  //__HAL_IWDG_RELOAD_COUNTER(&hiwdg1);

  new RunTime_c ;

  new CtrlProcess_c(2048,tskIDLE_PRIORITY+4,64,SignalLayer_c::HANDLE_CTRL);
  new TcpProcess_c(2048,tskIDLE_PRIORITY+5,64,SignalLayer_c::HANDLE_TCP);
  new HttpProcess_c(512,tskIDLE_PRIORITY+3,64,SignalLayer_c::HANDLE_HTTP);
  new TelnetProcess_c(512,tskIDLE_PRIORITY+3,64,SignalLayer_c::HANDLE_TELNET);
  new FtpProcess_c(1024,tskIDLE_PRIORITY+3,64,SignalLayer_c::HANDLE_FTP);

  new LonInterfaceProcess_c(512,tskIDLE_PRIORITY+5,64,SignalLayer_c::HANDLE_LON_INTERFACE);
  new LonTrafficProcess_c(2048,tskIDLE_PRIORITY+4,64,SignalLayer_c::HANDLE_LON_TRAFFIC);
  new LonSensorsDatabaseProcess_c(1024,tskIDLE_PRIORITY+3,64,SignalLayer_c::HANDLE_LON_SENSORS_DATABASE);
  new PwrProcess_c(1024,tskIDLE_PRIORITY+2,64,SignalLayer_c::HANDLE_POWER);

  vTaskStartScheduler();

  return 0;
}


static void prvSetupHardware( void )
{
  //SystemInit();
  //NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );



}

static void prvSetupHeap( void )
{
  



  //vPortDefineHeapRegions( xHeapRegions );
  baseManager.DefineHeapRegions(xHeapRegions);



  //vPortDefineHeapRegions( xHeapRegions );
  ccmManager.DefineHeapRegions(xCCMHeapRegions);

  ramD2Manager.DefineHeapRegions(xRAMD2HeapRegions);

  ramD2Manager.allowNullResult = true;

}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Supply configuration update enable
  */
 // HAL_PWREx_ConfigSupply(PWR_DIRECT_SMPS_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  //__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

//  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();
  __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSE);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE
                              |RCC_OSCILLATORTYPE_LSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSEState = RCC_LSE_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCO1SOURCE_PLL1QCLK, RCC_MCODIV_1);
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC | RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.PLL2.PLL2M = 5;
  PeriphClkInitStruct.PLL2.PLL2N = 48;
  PeriphClkInitStruct.PLL2.PLL2P = 5;
  PeriphClkInitStruct.PLL2.PLL2Q = 2;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_2;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOWIDE;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_RCC_RTC_ENABLE();
}

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x2000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  //HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_PRIV_RO_URO;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

void MX_IWDG1_Init(void)
{

  /* USER CODE BEGIN IWDG1_Init 0 */

  /* USER CODE END IWDG1_Init 0 */

  /* USER CODE BEGIN IWDG1_Init 1 */

  /* USER CODE END IWDG1_Init 1 */
  hiwdg1.Instance = IWDG1;
  hiwdg1.Init.Prescaler = IWDG_PRESCALER_128;
  hiwdg1.Init.Window = 4095;
  hiwdg1.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG1_Init 2 */

  /* USER CODE END IWDG1_Init 2 */


}

#ifdef __cplusplus
 extern "C" {
#endif

HAL_StatusTypeDef HAL_InitTick(uint32_t TickPriority)
{
  RCC_ClkInitTypeDef    clkconfig;
  uint32_t              uwTimclock;

  uint32_t              uwPrescalerValue;
  uint32_t              pFLatency;
/*Configure the TIM1 IRQ priority */
  if (TickPriority < (1UL << __NVIC_PRIO_BITS))
  {
  HAL_NVIC_SetPriority(TIM1_UP_IRQn, TickPriority ,0U);

  /* Enable the TIM1 global Interrupt */
  HAL_NVIC_EnableIRQ(TIM1_UP_IRQn);
    uwTickPrio = TickPriority;
    }
  else
  {
    return HAL_ERROR;
  }

  /* Enable TIM1 clock */
  __HAL_RCC_TIM1_CLK_ENABLE();

  /* Get clock configuration */
  HAL_RCC_GetClockConfig(&clkconfig, &pFLatency);

  /* Compute TIM1 clock */
  uwTimclock = 2*HAL_RCC_GetPCLK2Freq();

  /* Compute the prescaler value to have TIM1 counter clock equal to 1MHz */
  uwPrescalerValue = (uint32_t) ((uwTimclock / 1000000U) - 1U);

  /* Initialize TIM1 */
  htim1.Instance = TIM1;

  /* Initialize TIMx peripheral as follow:
  + Period = [(TIM1CLK/1000) - 1]. to have a (1/1000) s time base.
  + Prescaler = (uwTimclock/1000000 - 1) to have a 1MHz counter clock.
  + ClockDivision = 0
  + Counter direction = Up
  */
  htim1.Init.Period = (1000000U / 1000U) - 1U;
  htim1.Init.Prescaler = uwPrescalerValue;
  htim1.Init.ClockDivision = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;

  htim1.PeriodElapsedCallback = Tim10PeriodElapsedCallback;

  if(HAL_TIM_Base_Init(&htim1) == HAL_OK)
  {
    /* Start the TIM time Base generation in interrupt mode */
    return HAL_TIM_Base_Start_IT(&htim1);
  }

  /* Return function status */
  return HAL_ERROR;
}

/**
  * @brief  Suspend Tick increment.
  * @note   Disable the tick increment by disabling TIM1 update interrupt.
  * @param  None
  * @retval None
  */
void HAL_SuspendTick(void)
{
  /* Disable TIM1 update Interrupt */
  __HAL_TIM_DISABLE_IT(&htim1, TIM_IT_UPDATE);
}

/**
  * @brief  Resume Tick increment.
  * @note   Enable the tick increment by Enabling TIM1 update interrupt.
  * @param  None
  * @retval None
  */
void HAL_ResumeTick(void)
{
  /* Enable TIM1 Update interrupt */
  __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);
}

void TIM1_UP_IRQHandler(void)
{
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 0 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 0 */
  HAL_TIM_IRQHandler(&htim1);
  /* USER CODE BEGIN TIM1_UP_TIM10_IRQn 1 */

  /* USER CODE END TIM1_UP_TIM10_IRQn 1 */
}

void Tim10PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  HAL_IncTick();
}


void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    char *pcTaskName )
{
  printf("stack overflow in %s\n",pcTaskName);
  while(1) {}

}

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer,
                                    StackType_t **ppxIdleTaskStackBuffer,
                                    uint32_t *pulIdleTaskStackSize )
{
/* If the buffers to be provided to the Idle task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t __attribute((section(".DTCM"))) xIdleTaskTCB;
static StackType_t __attribute((section(".DTCM"))) uxIdleTaskStack[ configMINIMAL_STACK_SIZE ];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}
/*-----------------------------------------------------------*/

/* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so the
application must provide an implementation of vApplicationGetTimerTaskMemory()
to provide the memory that is used by the Timer service task. */
void vApplicationGetTimerTaskMemory( StaticTask_t **ppxTimerTaskTCBBuffer,
                                     StackType_t **ppxTimerTaskStackBuffer,
                                     uint32_t *pulTimerTaskStackSize )
{
/* If the buffers to be provided to the Timer task are declared inside this
function then they must be declared static - otherwise they will be allocated on
the stack and so not exists after this function exits. */
static StaticTask_t __attribute((section(".DTCM"))) xTimerTaskTCB;
static StackType_t __attribute((section(".DTCM"))) uxTimerTaskStack[ configTIMER_TASK_STACK_DEPTH ];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void vApplicationTickHook(void)
{
 /*  static bool state = false;

   if(state)
   {
    
     state = false;
     HAL_GPIO_WritePin(Led1_GPIO_Port,Led1_Pin,GPIO_PIN_RESET);
   }
   else
   {
     HAL_GPIO_WritePin(Led1_GPIO_Port,Led1_Pin,GPIO_PIN_SET);
     state = true;
   }
*/


}

void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef __cplusplus
}
#endif

