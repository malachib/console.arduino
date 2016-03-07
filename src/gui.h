#pragma once

#include "variants.h"

#include <Adafruit_ILI9341.h>
#include <Service.h>
#include <EventManager.h>

#include "gfx_primitives.h"
#include "gfx_touch.h"

class GUIService
{
protected:
  enum State
  {
    Initializing,
    Calibration,
    Active
  };

public:
  GUIService() {}

  static void begin();
  static void stateHandler();
  static void stateHandlerMonitor();
  static void stateHandlerCalibration();
  static void initializeActive();

  static State state;

};


class TouchCalibration
{
public:
  Rectangle screenBounds;

  TouchCalibration() {}
};

class TouchService : public TouchCalibration
{
  RegionResponder* regionResponder;

  LOCAL_EVENT(TouchService);
  //DECLARE_EVENT(Region) pressed;
  //DECLARE_EVENT(Region) released;
protected:
  virtual Vector3D getPoint() = 0;
public:
  Event pressed;
  Event released;

  void setRegionResponder(RegionResponder* regionResponder = NULL);
  void begin() { setRegionResponder(); }
  void reset(RegionResponder* regionResponder = NULL)
  {
    pressed.clear();
    released.clear();
    setRegionResponder(regionResponder);
  }
  void stateHandler();

  // TODO: improve event handler to actually pass in a parameter vs. just the sender
  Region* lastPressed;
  Vector3D lastPoint;

  static bool calibrated;
};

class AnalogTouchService : public TouchService
{
protected:
  virtual Vector3D getPoint();
};

class MenuService
{
public:
  static void begin();
  static void stateHandler();
  static void touchReleasedHandler(TouchService* ts);
};




extern GUIService gui;
extern AnalogTouchService touch;
extern Adafruit_ILI9341 tft;
extern MenuService _menu;
