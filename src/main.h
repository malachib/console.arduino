#pragma once

#include <Service.h>

class EEPROMService 
{
  struct Profile
  {
    bool isActive;
    unsigned schemaVersion : 7;
  };
  
  static Profile profile;

public:
  static void load();
  static void save(); 
  static void begin() { load(); } 
  
  static bool hasProfile() { return profile.isActive; }
};

extern EEPROMService eeprom;
