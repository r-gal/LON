#ifndef TRAFFIC_PROCESS_H
#define TRAFFIC_PROCESS_H



#include "LonDataDef.hpp"
class LonChannel_c;
#if USE_VIRTUAL_CHANNELS == 1
class LonChannelVirtual_c;
#endif


#if USE_SENSORS == 1
#include "LonSensors.hpp"
#endif

#if USE_TIMERS == 1
#include "LonTimers.hpp"
#endif

#if USE_COMMAND_LINK == 1
#include "LonCommands.hpp"
#endif

class LonTimeEvent_c : public TimeEvent_c
{
  public:

  void Action(SystemTime_st* time);
};


class LonTrafficProcess_c : public process_c
{
  public:
  #if USE_SENSORS == 1
  LonSensors_c* sensors;
  #endif
  private:

  #if USE_COMMAND_LINK == 1
  CommandLon_c commandLon;
  #endif

  uint8_t actChecked;

  uint8_t actCheckedChNo;
  uint8_t checkDevTimer;

  LonChannel_c* ch_p[NO_OF_CHANNELS];
  void SearchNewDevices(void);
  void IoCtrlSigHandle(LonIOdata_c* recSig_p);
  void IoTrafSigHandle(LonIOdata_c* recSig_p);
  void HandleDataTrAck(LonIOdata_c* recSig_p);
  void HandleIoReadAck(LonIOdata_c* recSig_p);  
  void HandleContainer(LonIOdata_c* recSig_p);

  #if USE_COMMAND_LINK == 1
  void HandleSetOutput(LonSetOutput_c* recSig_p);
  void HandleGetOutput(LonGetOutput_c* recSig_p);
  void HandleDeviceConfig(LonDeviceConfig_c* recSig_p);
  void GetDevList(LonGetDevList_c* recSig_p);
  void GetDevInfo(LonGetDevInfo_c* recSig_p);
  //void HandleGetStat(GetStat_c* recSig_p);
  #endif

  LonTimeEvent_c timeEvent;
  

  void ScanTmo(void);
  void StepChannelTimeouts(void);


  void CheckDevFromConfig(void);
  void CheckDevFromConfigAck(LonIOdata_c* recSig_p);

  void GetOutputsList(LonGetOutputsList_c* recSig_p);
  #if USE_SENSORS == 1
  void GetSensorsValues(LonGetSensorsValues_c* recSig_p);
  #endif


  public :
  LonDevice_c* GetDevByLadr(uint32_t lAdr);
  LonDevice_c* GetDevice(uint8_t chNo,uint8_t sAdr);

  void DeleteDevice(LonDevice_c* dev_p);

  void RunForAll(Function_type function, void* userPtr);

  void HandleDataSw(LonDevice_c* dev_p, uint8_t noOfEvents, uint8_t* eventsArray);

  void SendFrame(LonDevice_c* dev_p, uint8_t bytesNo, uint8_t* buffer);


  LonTrafficProcess_c(uint16_t stackSize, uint8_t priority, uint8_t queueSize, HANDLERS_et procId);

  void main(void);

  #if USE_VIRTUAL_CHANNELS == 1
  LonChannelVirtual_c* AssignNewVirtualChannel(LonDevice_c* dev_p);
  void CleanVirtualChannel(uint8_t chNo) { ch_p[chNo] = NULL; }
  #endif


};

class LonTmoCounter_c
{
  
  LonTmoCounter_c* next;
  LonTmoCounter_c* prev;

  uint8_t checkDevTimer;

  public:

  LonTmoCounter_c(void );
  ~LonTmoCounter_c(void );


  LonTmoCounter_c* Step(LonTrafficProcess_c* traf_p);

  static LonTmoCounter_c* firstTmo;


  uint32_t lAdr;
  uint32_t rLAdr;
  uint32_t rLAdr2;
  uint32_t counter;
  uint8_t port;
  uint8_t rPort;
  uint8_t rPort2;

};





#endif