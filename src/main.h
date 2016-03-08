#pragma once

#include <Service.h>
#include <taskmanager.h>

class EEPROMService 
{
  struct Profile
  {
    unsigned _isActive : 1;
    unsigned schemaVersion : 7;
    
    bool isActive() { return _isActive ^ 1; }
    
    Profile() {}
  };
  
  static Profile profile;

public:
  static void load();
  static void save(); 
  static void begin() { load(); } 
  
  static bool hasProfile() { return profile.isActive(); }
};

extern EEPROMService eeprom;
extern util::ScheduleManager sm;
