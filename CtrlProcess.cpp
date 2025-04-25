 #include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"


#include "CtrlProcess.hpp"
//#include "ff_stdio.h" 
#include "RunTimeStats.hpp"

#include "FileSystem.hpp"



/*
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rtc.h"
#include "stm32f4xx_dma.h"
#include "misc.h"

*/


void vFunction100msTimerCallback( TimerHandle_t xTimer )
{
  //dispActTimSig.Send();

}

CtrlProcess_c::CtrlProcess_c(uint16_t stackSize, uint8_t priority, uint8_t queueSize, HANDLERS_et procId) : process_c(stackSize,priority,queueSize,procId,"CONFIG")
{

 // TimerHandle_t timer = xTimerCreate("",pdMS_TO_TICKS(500),pdTRUE,( void * ) 0,vFunction100msTimerCallback);
 // xTimerStart(timer,0);

  

}

void CtrlProcess_c::main(void)
{
  RunTime_c::Start();
  timeUnit.Init();
  bootUnit.Init(TimeUnit_c::GetHrtc());
  rng.Init();
  ethernet.Init();

  sdCard.Init();
  #if DEBUG_PROCESS > 0
  printf("Ctrl proc started \n");
  #endif



  /* SD card test */

  FindData_c* findData = new FindData_c;

  bool res;

  if(findData->FindFirst((char*)"") == true)
  { 
    do
    {
      printf(findData->entryData.name);
      printf("\n");

      res = findData->FindNext();
    } while(res == true );
  }

  delete findData;


  while(1)
  {
    releaseSig = true;
    RecSig();
    uint8_t sigNo = recSig_p->GetSigNo();

    

    switch(sigNo)
    {
      case SIGNO_SDIO_CardDetectEvent:
        sdCard.CardDetectEvent();
        releaseSig = false;
        break;
      default:
      break;

    }
    if(releaseSig)
     {
      delete  recSig_p;
       }
 
  }


}

