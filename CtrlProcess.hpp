#ifndef CTRL_PROCESS_H
#define CTRL_PROCESS_H

#include "SignalList.hpp"
#include "CommonDef.hpp"

#if LON_USE_ETHERNET == 1
#include "Ethernet.hpp"
#endif

#if LON_USE_SDCARD == 1
#include "sdCard_stm32h7xx.hpp"
#endif

#include "CommandSystem.hpp"

#if LON_USE_RNG == 1
#include "RngClass.hpp"
#endif
#if CONF_USE_TIME == 1
#include "TimeClass.hpp"
#endif

#if LON_BOOTUNIT == 1
#include "BootUnit.hpp"
#endif


class CtrlProcess_c : public process_c
{
  CommandSystem_c commandSystem;

  #if LON_USE_ETHERNET == 1
  Ethernet_c ethernet;
  #endif
  #if LON_USE_SDCARD == 1
  SdCard_c sdCard;
  #endif
  #if LON_USE_RNG == 1
  RngUnit_c rng;
  #endif
  #if CONF_USE_TIME == 1
  TimeUnit_c timeUnit;
  #endif
  #if LON_BOOTUNIT == 1
  BootUnit_c bootUnit;
  #endif

 

  public :

  CtrlProcess_c(uint16_t stackSize, uint8_t priority, uint8_t queueSize, HANDLERS_et procId);

  void main(void);

  

};



#endif