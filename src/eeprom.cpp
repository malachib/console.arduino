#include <fact/lib.h>
#include <Console.h>

#include "gfx_primitives.h"
#include "main.h"
#include "gui.h"


EEPROMService::Profile EEPROMService::profile;

#define ADDR_START 0
#define ADDR_PROFILE ADDR_START
#define ADDR_SCREENBOUNDS (ADDR_PROFILE) + sizeof(Profile)

EEPROMService eeprom;

#ifdef __AVR__
#include <EEPROM.h>

void EEPROMService::load()
{
#ifdef DEBUG
  uint8_t value = EEPROM.read(ADDR_PROFILE);
  
  Serial.print(F("EEPROM read: "));
  Serial.print(value, DEC);
  Serial.println();
#endif
  EEPROM.get(ADDR_PROFILE, profile);
  EEPROM.get(ADDR_SCREENBOUNDS, touch.screenBounds);
}


void EEPROMService::save()
{
#ifdef DEBUG
  cout.println(F("EEPROM write"));
#endif
  profile._isActive = 0; // EEPROM we need to store this particular value in reverse, cuz default is 1
  EEPROM.put(ADDR_PROFILE, profile);
  EEPROM.put(ADDR_SCREENBOUNDS, touch.screenBounds);
}


#elif SAMD_SERIES

#include <FlashStorage.h>

FlashStorage(flash_profile, EEPROMService::Profile);

void EEPROMService::load()
{
  flash_profile.read(&profile);
  //profile._isActive = 1; // remember it's reversed, so this means inanctive
}

void EEPROMService::save()
{
  flash_profile.write(profile);
}
#else
#error MPU not supported
#endif
