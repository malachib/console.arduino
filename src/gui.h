#pragma once

#include "variants.h"

#include <Adafruit_ILI9341.h>
#include <Service.h>

#include "gfx_primitives.h"

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


class TouchService
{
public:
  static void begin();
  static void stateHandler();
};

extern GUIService gui;
extern TouchService touch;
extern Adafruit_ILI9341 tft;
