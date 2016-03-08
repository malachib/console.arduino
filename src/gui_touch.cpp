// this is app inspecific code area, eventually to end up in a library

#include "gui.h"
#include "gfx_touch.h"
#include <fact/lib.h>


// turns out my Adafruit doesn't have SPI touch screen breakout (looks like clone does thoough,
// but we'll get there later)
//#include <Adafruit_STMPE610.h>
#include <TouchScreen.h>

#ifdef ANALOG_TOUCHSCREEN
#define YP A2  // must be an analog pin, use "An" notation!
#define XM A3  // must be an analog pin, use "An" notation!
#define YM 5   // can be a digital pin
#define XP 4   // can be a digital pin

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
  Region* region = regionResponder.find(vector);
}


bool isPressed = false;

void TouchService::setRegionResponder(RegionResponder* regionResponder)
{
  //released.clear();
  //pressed.clear();

  this->regionResponder = regionResponder;
}


void TouchService::stateHandler()
{
  Vector3D p = getPoint();

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > ts.pressureThreshhold)
  {
    if(calibrated)
    {
  #ifdef DEBUG
      static uint8_t thinner = 0;

      if(thinner++ % 100 == 0)
      {
        Serial << F("Precalibrated: ") << p;
        Serial.println();
      }
  #endif
      auto width = screenBounds.size.x;
      auto height = screenBounds.size.y;

      //p -= screenBounds.origin;

      p.x -= screenBounds.origin.x;
      p.y -= screenBounds.origin.y;

      //p.x -= upperLeft.x;
      //p.y -= upperLeft.y;

      // FIX: temporarily hardcode screen dimensions here
      p.x = ((uint32_t)p.x) * 240 / width;
      p.y = ((uint32_t)p.y) * 320 / height;

      // TODO: calibration is far from perfect.  We get out of bounds a lot (negative x, etc)
      // so compensate for that.  However for short term, we're OK with that
    }

    // only record points for actual press, it seems (and makes sense)
    // that release events don't give solid X/Y coordinates
    lastPoint = p;

#ifdef DEBUG
    static uint8_t thinner = 0;

    if(thinner++ % 100 == 0)
    {
      Serial << F("Touch found at: ") << p;
      Serial.println();
    }
#endif

    lastTouchedTime = millis();
    
    if(!isPressed)
    {
      lastPressedTime = lastTouchedTime;
      isPressed = true;
    }
    
    touching(this);
    
    // this is a "press" (aka mousedown) event
    if(regionResponder)
    {
      // only re-press if we move to a different region from before
      Region* r = regionResponder->find(p);
      if(r != lastPressed)
      {
        // TODO: once we get an extra parameter for the event system,
        // this will go more smoothly
        lastPressed = r;
        pressed(this);
      }
    }
    else
    {
      // we can operate without a region responder in which
      // case all press/releases are put forward unfiltered
      lastPressed = NULL;
      pressed(this);
    }
  }
  else
  {
    // wait 100ms before servicing a button press, I guess this is
    // kind of a de-bounce code
    if(isPressed && millis() > (lastTouchedTime + 50) )
    {
      // this is a "release" (aka mouseup) event
      //Region* r = regionResponder->find(p);
      auto r = lastPressed;
      if(r || !regionResponder)
      {
        // last pressed will already be populated from press
        // *theoretically* it's impossible to have a release
        // on a different position than a press....
        //lastPressed = r;
#ifdef DEBUG
        Serial << F("Touch released at: ") << lastPoint;
        Serial.println();
#endif
        released(this);
#ifdef DEBUG2
        Serial << F("Touch released at: ") << p.x << ',' << p.y << ',' << p.z;
        Serial.println();
#endif
      }
#ifdef DEBUG2
        Serial.println("Got here 2");
#endif
      isPressed = false;
    }
  }
}


bool TouchCalibration::calibrated = false;
Rectangle TouchCalibration::screenBounds;

Vector3D AnalogTouchService::getPoint()
{
  auto p = ts.getPoint();

  return p;
}
