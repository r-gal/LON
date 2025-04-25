 #include <stdio.h>
#include <stdlib.h>
#include <cstring>

#include "BootUnit.hpp"
#include "Ethernet.hpp"

#include "EthernetTcp.hpp"

#include "FileSystem.hpp"

#ifdef __cplusplus
 extern "C" {
#endif

extern void JumpToApplication(unsigned int);

#ifdef __cplusplus
}
#endif


uint8_t __attribute((section(".APP_FLASH1"))) app1;
uint8_t __attribute((section(".APP_FLASH2"))) app2;

AppInfo_st __attribute((section(".APP_INFO1"))) appInfo1;
AppInfo_st __attribute((section(".APP_INFO2"))) appInfo2;

const AppInfo_st __attribute((section(".APP_INFO_ACT"))) appInfoAct = {.versionName = "VER 0.95 "__DATE__" "__TIME__ };

const char* versionName = appInfoAct.versionName;

BootUnit_c::BootUnit_c(void): comBoot(this)
{
  


}

void BootUnit_c::Init(RTC_HandleTypeDef* hrtc_)
{
  if(hrtc_ != nullptr)
  {
    hrtc = hrtc_;
  }
  else
  {
    hrtc = new RTC_HandleTypeDef;
    #ifdef STM32H725xx

    __HAL_RCC_RTC_CLK_ENABLE();
    #else
    __HAL_RCC_PWR_CLK_ENABLE();
    #endif
    memset(hrtc,0,sizeof(RTC_HandleTypeDef));
    hrtc->Instance = RTC;
  }

}


comResp_et Com_boot::Handle(CommandData_st* comData_)
{
  
  int actionIdx = FetchParameterString(comData_,"ACTION");

  uint32_t idx;
  bool idxValid = FetchParameterValue(comData_,"IDX",&idx,1, 2);

  if(actionIdx == -1)
  {
    return COMRESP_NOPARAM;
  }
  if(strcmp(comData_->buffer+comData_->argValPos[actionIdx],"INFO") == 0)
  {
    char* buffer = new char[512];
    bootUnit->PrintStatus(buffer);
    Print(comData_->commandHandler,buffer);
    delete[] buffer;
    return COMRESP_OK;
  }
  else if(strcmp(comData_->buffer+comData_->argValPos[actionIdx],"REBOOT") == 0)
  {
    if(idxValid == true)
    {
      bootUnit->Reboot(idx);
    }
    else
    {
      bootUnit->Reboot(0xFF);
    }
    
    return COMRESP_OK;
  }
  else if(strcmp(comData_->buffer+comData_->argValPos[actionIdx],"LOAD") == 0)
  {
    if(idxValid == false)
    {
      return COMRESP_NOPARAM;
    }

    int fileIdx =  FetchParameterString(comData_,"FILE");
    if(fileIdx == -1)
    {
      return COMRESP_NOPARAM;
    }

    if(bootUnit->WriteToFlash(comData_->buffer+comData_->argValPos[fileIdx],idx) == true)
    {
      return COMRESP_OK;
    }
    else
    {
      return COMRESP_VALUE;
    }
  }

  else if(strcmp(comData_->buffer+comData_->argValPos[actionIdx],"BOOTLOADER") == 0)
  {
    int fileIdx =  FetchParameterString(comData_,"FILE");
    if(fileIdx == -1)
    {
      return COMRESP_NOPARAM;
    }

    if(bootUnit->WriteToFlash(comData_->buffer+comData_->argValPos[fileIdx],0) == true)
    {
      return COMRESP_OK;
    }
    else
    {
      return COMRESP_VALUE;
    }


  }

  else if(strcmp(comData_->buffer+comData_->argValPos[actionIdx],"DEFAULT") == 0)
  {
    if(idxValid == false)
    {
      return COMRESP_NOPARAM;
    }
    bootUnit->SetDefault(idx);
    return COMRESP_OK;
  }
  else
  {
    return COMRESP_VALUE;
  }
}

void BootUnit_c::PrintStatus(char* buffer)
{
 uint32_t actOrder = HAL_RTCEx_BKUPRead(hrtc,0);

 strcpy(buffer,"VERSIONS: \n");

 strcat(buffer," CURR:  ");
 strncat(buffer, appInfoAct.versionName ,32);
 strcat(buffer,"\n");

  strcat(buffer," DEF IDX: ");
 if((actOrder & BOOT_DEFAULT_MASK) == BOOT_DEFAULT1)
 {
    strcat(buffer,"1\n");
 }
 else if((actOrder & BOOT_DEFAULT_MASK) == BOOT_DEFAULT2)
 {
    strcat(buffer,"2\n");
 }

 strcat(buffer," APPL1: ");
 strncat(buffer, appInfo1.versionName ,32);
 strcat(buffer,"\n");

 strcat(buffer," APPL2: ");
 strncat(buffer,appInfo2.versionName ,32);
 strcat(buffer,"\n");


}
#define FILE_BUFFER_SIZE 512

bool BootUnit_c::WriteToFlash(char* fileName,uint32_t idx)
{
  uint32_t flashAdr;
  uint32_t firstSector;
  if(idx == 0)
  {
    /* bootloader update */
    flashAdr = 0x08000000;
    firstSector = FLASH_SECTOR_0;    
  }
  else if(idx == 1)
  {
    flashAdr = (uint32_t)&app1;
    firstSector = APPLICATION1_SECTOR;
    
  }
  else if(idx == 2)
  {
    flashAdr = (uint32_t)&app2;
    firstSector = APPLICATION2_SECTOR;
  }
  else
  {
    return false;
  }

  char* fileNamePatch = new char[ strlen(fileName)+8];
  strcpy(fileNamePatch,"/BOOT/");
  strcat(fileNamePatch,fileName);

  File_c * file = FileSystem_c::OpenFile( fileNamePatch,"r");
  delete [] fileNamePatch;

  if(file != nullptr)
  {
    uint32_t fileSize = file->GetSize();
    bool result = false;
    HAL_FLASH_Unlock();
    /* erase sectors */
    uint8_t sectorsCount = 1 + fileSize/(128*1024);

    FLASH_EraseInitTypeDef pEraseInit;
    pEraseInit.Banks = 1;
    pEraseInit.NbSectors = 1;
    pEraseInit.Sector = firstSector;
    pEraseInit.TypeErase = FLASH_TYPEERASE_SECTORS;
    pEraseInit.VoltageRange = FLASH_VOLTAGE_RANGE_3;

    vPortEnterCritical();
    
    uint32_t sectorError;
    printf("Try to errase sector %d\n", pEraseInit.Sector);
    HAL_FLASHEx_Erase(&pEraseInit, &sectorError);
    printf("Errase result = %x\n",sectorError);
    



    pEraseInit.Sector++;
     printf("Try to errase sector %d\n", pEraseInit.Sector);
    HAL_FLASHEx_Erase(&pEraseInit, &sectorError);
    printf("Errase result = %x\n",sectorError);

    vPortExitCritical();

    if(sectorError == 0xFFFFFFFF)
    {
      /* program flash */
      uint32_t* fileBuffer = new uint32_t[FILE_BUFFER_SIZE/4];
      uint32_t bytesWritten = 0;
      uint32_t bufferReadIdx = FILE_BUFFER_SIZE;

      result = true;


      int noOfFlashWords = fileSize /(4* FLASH_NB_32BITWORD_IN_FLASHWORD);

      printf("File size = %d, flashWords = %d\n",fileSize,noOfFlashWords);

      while(bytesWritten < fileSize)
      { 
        if(bufferReadIdx >= FILE_BUFFER_SIZE)
        {
           file->Read((uint8_t*)fileBuffer,FILE_BUFFER_SIZE);
           printf("read buffer\n");
           bufferReadIdx = 0;
        }
        uint32_t srcAddr = (uint32_t)(fileBuffer) + bufferReadIdx;
        uint32_t dstAddr = flashAdr+bytesWritten;


        vPortEnterCritical();

        HAL_StatusTypeDef status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_FLASHWORD,dstAddr,srcAddr);

        vPortExitCritical();

        for(int i=0;i<FLASH_NB_32BITWORD_IN_FLASHWORD;i++)
        {
          uint32_t* fileData_p = ((uint32_t*)srcAddr);
          uint32_t* flashData_p = ((uint32_t*)dstAddr);
          uint32_t fileData = fileData_p[i];
          uint32_t flashData = flashData_p[i];

          if(fileData!= flashData) 
          {
            printf("verification failed %d:%d, src=%x, dst=%x\n",bytesWritten,i,fileData,flashData);

          }



        }


        bufferReadIdx += 4* FLASH_NB_32BITWORD_IN_FLASHWORD;
        bytesWritten += 4* FLASH_NB_32BITWORD_IN_FLASHWORD;

        

        printf("Write %d bytes, total written = %d, src=%x, dst=%x status =%d\n",4* FLASH_NB_32BITWORD_IN_FLASHWORD,bytesWritten,srcAddr,dstAddr,status);




        if(status != HAL_OK)
        {
          result = false;
          break;
        }
      }

      delete[] fileBuffer;      
    }

    HAL_FLASH_Lock();

    

    file->Close();
    return result;
  }
  else
  {
    return false;
  }
}

bool BootUnit_c::SetDefault(uint32_t idx)
{
  uint32_t newOrder = 0;

  if(idx == 1 )
  {
    newOrder = 0xBEAF0000 | BOOT_DEFAULT1;
  } 
  else if(idx == 2 )
  {
    newOrder = 0xBEAF0000 | BOOT_DEFAULT2;
  } 
  else
  {
    return false;
  }

  HAL_PWR_EnableBkUpAccess();
  HAL_RTCEx_BKUPWrite(hrtc,0,newOrder);
  HAL_PWR_DisableBkUpAccess();
  return true;
}


void vDelayedRestart( TimerHandle_t xTimer )
{
  unsigned int VectorTableAddress;
  unsigned int* p; 
  
  p = (unsigned int*)(0x08000000);
  
 // VectorTableAddress = p[0];
 VectorTableAddress =(unsigned int)p;



  taskDISABLE_INTERRUPTS();
  vTaskSuspendAll();

  HAL_NVIC_SystemReset();

  //JumpToApplication(VectorTableAddress);
}

void BootUnit_c::Reboot(uint32_t idx)
{

  uint32_t newOrder = HAL_RTCEx_BKUPRead(hrtc,0);
  if((newOrder & 0xFFFF0000) == 0xBEAF0000)
  {
    newOrder = newOrder & ~BOOT_ORDER_MASK;
    if(idx == 1)
    {     
      newOrder |= BOOT_ORDER_1;
    }
    else if(idx == 2)
    {
      newOrder |= BOOT_ORDER_2;
    }
    else if((newOrder & BOOT_DEFAULT_MASK) == BOOT_DEFAULT2)
    {
      newOrder |= BOOT_ORDER_2;
    }
    else
    {
      newOrder |= BOOT_ORDER_1;
    }
  }
  else
  {    
    if(idx == 1)
    {     
      newOrder = 0xBEAF0000 | BOOT_DEFAULT1 | BOOT_ORDER_1;
    }
    else
    {
      newOrder = 0xBEAF0000 | BOOT_DEFAULT2 | BOOT_ORDER_2;
    }
  }
  HAL_PWR_EnableBkUpAccess();
  HAL_RTCEx_BKUPWrite(hrtc,0,newOrder);
  HAL_PWR_DisableBkUpAccess();

  /* disconnect all sockets */

  Socket_c* socket = Socket_c::GetSocket(23,IP_PROTOCOL_TCP);
  if(socket != nullptr)
  {
    ((SocketTcp_c*) socket) ->DisconnectAllChild();
  }

  /*delayed restart, switch to another task to enable disconnect pending connection */
  TimerHandle_t timer = xTimerCreate("",pdMS_TO_TICKS(500),pdTRUE,( void * ) 0,vDelayedRestart);
  xTimerStart(timer,0);

}