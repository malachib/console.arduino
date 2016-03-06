// this is app inspecific code area, eventually to end up in a library

#include "gui.h"
#include "gfx_touch.h"


// turns out my Adafruit doesn't have SPI touch screen breakout (looks like clone does thoough,
// but we'll get there later)
//#include <Adafruit_STMPE610.h>
#include <TouchScreen.h>

#ifdef ANALOG_TOUCHSCREEN
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 8   // can be a digital pin
#define XP 9   // can be a digital pin

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
#else
// The STMPE610 uses hardware SPI on the shield, and #8
#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);
#endif

AnalogTouchService touch;

void TouchActionResponder::touch(_Vector3D<uint16_t> vector)
{
  Region* region = regionResponder.find(vector.x, vector.y);
}

void touch_test2(_Vector3D<uint16_t> vector)
{
  //Serial << F("TEST");
  //Serial.println();
  vector.x++;
}

void touch_test()
{
  TSPoint point;
  _Vector3D<uint16_t> p2 = point;
  touch_test2(point);
  //_Vector<uint32_t> p1 = point;
  //_Vector3D<uint16_t> p3 = p1;
}


bool isPressed = false;

void TouchService::begin(RegionResponder* regionResponder)
{
  this->regionResponder = regionResponder;
}


void TouchService::stateHandler()
{
  TSPoint p = ts.getPoint();

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > ts.pressureThreshhold)
  {
    isPressed = true;
  }
  else
  {
    if(isPressed)
    {
      // this is a "release" (aka mouseup) event
      Region* r = regionResponder->find(p);

      if(r != NULL)
      {
        lastPressed = r;
        released(this);
      }
    }
  }
}


Vector3D AnalogTouchService::getPoint()
{
  return ts.getPoint();
}
