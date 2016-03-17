#pragma once

#include <Service.h>
#include <taskmanager.h>

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

extern EEPROMService eeprom;
extern util::ScheduleManager sm;
