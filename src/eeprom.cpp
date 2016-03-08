#ifdef __AVR__
#include <EEPROM.h>
#include <fact/lib.h>

#include "gfx_primitives.h"
#include "main.h"
#include "gui.h"


EEPROMService::Profile EEPROMService::profile;

#define ADDR_START 0
#define ADDR_PROFILE ADDR_START
#define ADDR_SCREENBOUNDS (ADDR_PROFILE) + sizeof(Profile)

EEPROMService eeprom;

void EEPROMService::load()
{
  EEPROM.get(ADDR_PROFILE, profile);
  EEPROM.get(ADDR_SCREENBOUNDS, touch.screenBounds);
}


void EEPROMService::save()
{
  EEPROM.put(ADDR_PROFILE, profile);
  EEPROM.put(ADDR_SCREENBOUNDS, touch.screenBounds);
}


#endif
