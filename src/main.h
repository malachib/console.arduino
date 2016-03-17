#pragma once

#include <Service.h>
#include <taskmanager.h>
#include <Console.h>

#include "features.h"

class EEPROMService 
{
public:
  static void load();
  static void save(); 
  static void begin() { load(); } 
  
  static bool hasProfile() { return profile.isActive(); }

  struct Profile
  {
    unsigned _isActive : 1;
    unsigned schemaVersion : 7;
    
#if SAMD_SERIES
    bool isActive() { return _isActive; }
#else
    bool isActive() { return _isActive ^ 1; }
#endif
    
    Profile() {}
  };
  
private:
  static Profile profile;
};


class ConsoleService
{
  class MainMenu : public util::Menu
  {
  public:
    MainMenu();
    
  } menu;
  
  util::ConsoleMenu console;
  
public:
  ConsoleService() : console(&menu)
  {
    
  }
  
  void stateHandler() { console.handler(); }
};

extern ConsoleService console;
extern EEPROMService eeprom;
extern util::ScheduleManager sm;
