#ifndef LON_COMMANDS_H
#define LON_COMMANDS_H


#include "commandHandler.hpp"

class Com_getdevlist : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"getdevlist"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_configdevice : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"configdevice"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_formatconfig : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"formatconfig"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_defragconfig : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"defragconfig"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_printdatabase : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"printdatabase"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_setoutput : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"setoutput"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_replacedevice : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"replacedevice"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_printdevice : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"printdevice"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_setpwm : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"setpwm"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_printtimers : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"printtimers"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_settimer : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"settimer"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_formattimers : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"formattimers"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_readconfig : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"readconfig"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_writeconfig : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"writeconfig"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};

class Com_rainsensor : public Command_c
{
  public:
  char* GetComString(void) { return (char*)"rainsensor"; }
  void PrintHelp(CommandHandler_c* commandHandler ){}
  comResp_et Handle(CommandData_st* comData_);
};


class CommandLon_c :public CommandGroup_c
{

  Com_getdevlist devlist;
  Com_configdevice configdevice;
  Com_formatconfig formatconfig;
  Com_defragconfig defragconfig;
  Com_printdatabase printdatabase;
  Com_setoutput setoutput;
  Com_replacedevice replacedevice;

  Com_printdevice printdevice;
  Com_setpwm setpwm;
  Com_printtimers printtimers;
  Com_settimer settimer;
  Com_formattimers formattimers;

  Com_readconfig readconfig;
  Com_writeconfig writeconfig;

  Com_rainsensor rainsensor;


  public:
};





























#endif