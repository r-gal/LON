#ifndef LonSensors_cLASS_H
#define LonSensors_cLASS_H

#include "SignalList.hpp"

class LonTrafficProcess_c;




class LonSensors_c : public SignalLayer_c
{
  static LonTrafficProcess_c* trafProc_p;

  static void ReadSensor(LonDevice_c* dev_p,void* userPtr);
  static void SendRequest(LonDevice_c* dev_p, uint8_t port);

  void handleDataPress(LonDevice_c* dev_p, uint8_t port, uint8_t* sensorData);
  void handleDataHig(LonDevice_c* dev_p, uint8_t port, uint8_t* sensorData);

  void CheckSensorAlarm(LonDevice_c* dev_p,uint8_t portNo);
 
  
  public:

  LonSensors_c(LonTrafficProcess_c* trafProc_p_);

  void ScanSensors(LonTime_c* recSig_p);
  void HandleSensorData(LonDevice_c* dev_p, uint8_t port, uint8_t* sensorData);



};

#define NO_OF_RAIN_SENSORS 1

class LonRainSensor_c
{

   int accRain;
   float scale;

   int lastHour[12];
   int lastHourIdx;

   int lastHours[48];
   int lastHoursIdx;

  public:

    LonRainSensor_c(void)
    {
      scale = 0.2794;

      accRain = 0;

      for(int i=0;i<12;i++)
      {
        lastHour[i] = 0;
      }
      lastHourIdx = 0;


      for(int i=0;i<48;i++)
      {
        lastHours[i] = 0;
      }
      lastHoursIdx = 0;

    }

    static void HandleEvent(int idx);
    static void SendAcumulatedData(bool fullHourIndicator);

    static void GetRainStats(LonGetRainStats_c* recSig_p);

};



#endif