#pragma once

#include "gfx_primitives.h"

// reports whether a given point occurs within a set of regions
class RegionResponder
{
  Region** regions;
  uint8_t regionCount;

public:
  RegionResponder(Region** regions, uint8_t regionCount) :
    regions(regions), regionCount(regionCount)
  {

  }

  Region* find(uint16_t x, uint16_t y)
  {
    Region** region = regions;

    for(int i = regionCount; i-- > 0;)
    {
      auto r = *region;
      if(x >= r->getX() && x <= r->getBoundX() &&
        y >= r->getY() && y <= r->getBoundY())
      {
        return r;
      }

      region++;
    }

    return NULL;
  }

  Region* find(_Vector<uint16_t> vector)
  {
    return find(vector.x, vector.y);
  }
};


// responds to durations (and eventually patterns, like double-tap, etc)
// of touches
class TouchActionResponder
{
  uint32_t lastTouchTime;
  Region* lastTouchedRegion;

  RegionResponder& regionResponder;

public:
  TouchActionResponder(RegionResponder& regionResponder) :
    regionResponder(regionResponder)
  {

  }
  // only call this when touch threshold has been reached, determined externally
  void touch(_Vector3D<uint16_t> vector);

};

class TouchResponder
{
public:
  // only call this when touch threshold has been reached, determined externally
  void touch(_Vector3D<uint16_t> vector);
};


class TouchCalibration
{
public:
  static Rectangle screenBounds;
  static bool calibrated;

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
  // fires continuously when touches are present
  Event touching;
  
  // fires when a new region is pressed
  Event pressed;
  
  // fires when finger is lifted off previously pressed region
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
  uint32_t lastPressedTime;
  uint32_t lastTouchedTime;
  Vector3D lastPoint;
};

class AnalogTouchService : public TouchService
{
protected:
  virtual Vector3D getPoint();
};
