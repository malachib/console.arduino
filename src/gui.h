#pragma once

#include "variants.h"

#include <Adafruit_ILI9341.h>
#include <Service.h>

class GUIService
{
public:
  GUIService() {}

  static void begin();
  static void stateHandler();
};


class MenuService
{
public:
  static void begin();
  static void stateHandler();
};

extern GUIService gui;
extern Adafruit_ILI9341 tft;
