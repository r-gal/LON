

#ifndef BOOT_CLASS_C
#define BOOT_CLASS_C

#include "SignalList.hpp"
#include "CommonDef.hpp"
#include "commandHandler.hpp"

#define BOOT_DEFAULT_MASK 0x0003
#define BOOT_DEFAULT1     0x0001
#define BOOT_DEFAULT2     0x0002

#define BOOT_ORDER_MASK 0x000C
#define BOOT_ORDER_1 0x0004
#define BOOT_ORDER_2 0x0008

class BootUnit_c;

struct AppInfo_st
{

  char versionName[32];
  uint8_t reserved[32];

};

class Com_boot : public Command_c
{
  BootUnit_c* bootUnit;
  public:
  char* GetComString(void) { return (char*)"boot"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
  Com_boot(BootUnit_c* bootUnit_) { bootUnit = bootUnit_; }

};

class BootUnit_c
{
  RTC_HandleTypeDef* hrtc;
  Com_boot comBoot;

  public:
  void Init(RTC_HandleTypeDef* hrtc_);

  BootUnit_c(void);

  void PrintStatus(char* buffer);
  bool WriteToFlash(char* fileName, uint32_t idx);
  bool SetDefault(uint32_t idx);
  void Reboot(uint32_t idx);
  

};



#endif