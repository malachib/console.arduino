#pragma once

#include "variants.h"

#include <Adafruit_ILI9341.h>
#include <Service.h>
#include <EventManager.h>

#include "gfx_primitives.h"
#include "gfx_touch.h"

#include "states.h"

class GUIService
{
protected:
public:
  enum State
  {
    Initializing,
    Calibration,
    Menu,
    Active
  };

  GUIService() {}

  static void begin();
  static void stateHandler();
  static void stateHandlerMonitor();
  static void stateHandlerCalibration();
  static void initializeActive();

  static State state;

};



#define LARGE_FONT

#ifdef LARGE_FONT
#define ROW_HEIGHT 16
#define COLUMN_WIDTH 12
#else
#define ROW_HEIGHT 8
#define COLUMN_WIDTH 6
#endif
#define SCREEN_HEIGHT 320
#define SCREEN_WIDTH 240

#define ROWS (SCREEN_HEIGHT / ROW_HEIGHT)
#define COLUMNS (SCREEN_WIDTH / COLUMN_WIDTH)



extern GUIService gui;
extern AnalogTouchService touch;
extern Adafruit_ILI9341 tft;
extern MenuService _menu;
extern SubState subState;
