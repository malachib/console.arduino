#pragma once

#include "variants.h"

#include <Adafruit_ILI9341.h>
#include <Service.h>
#include <EventManager.h>

#include "gfx_primitives.h"
#include "gfx_touch.h"

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
  RegionResponder* regionResponder;

  // TODO: improve event handler to actually pass in a parameter vs. just the sender
  Region* lastPressed;

  LOCAL_EVENT(TouchService);
  //DECLARE_EVENT(Region) pressed;
  //DECLARE_EVENT(Region) released;
  Event pressed;
  Event released;

protected:
  virtual Vector3D getPoint() = 0;
public:
  void begin(RegionResponder* regionResponder);
  void stateHandler();
};

class AnalogTouchService : public TouchService
{
protected:
  virtual Vector3D getPoint();
};

extern GUIService gui;
extern AnalogTouchService touch;
extern Adafruit_ILI9341 tft;
