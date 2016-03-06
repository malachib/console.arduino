#include <SPI.h> // stokes auto-lib resolver for platformio
// turns out my Adafruit doesn't have SPI touch screen breakout (looks like clone does thoough,
// but we'll get there later)
//#include <Adafruit_STMPE610.h>
#include <TouchScreen.h>

#include "variants.h"
#include "gui.h"
#include "monitor.h"

#ifdef T3_9341
#include <ILI9341_t3.h>
#else
#include <Adafruit_ILI9341.h>
#endif

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

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
#ifdef T3_9341
ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);
#else
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
#endif

GUIService gui;

void GUIService::begin()
{
#ifndef ADAFRUIT_ILI9341
  // chinese clone version needs LCD explitly turned on, although
  // we could probably tie the line high - WITH A RESISTOR - if I understand my
  // electronics right
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
#endif

  tft.begin();

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.println("Hello World!");

#ifndef ANALOG_TOUCHSCREEN
  if (!ts.begin()) {
    tft.println("Couldn't start touchscreen controller");
  }
#endif
}

void GUIService::stateHandler()
{
  while(monitor.stream->available())
  {
    int ch = monitor.stream->read();
    if(ch == 13)
      tft.println();
    else
      tft.print((char)ch);
  }
}
