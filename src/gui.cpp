#include <SPI.h> // stokes auto-lib resolver for platformio
#include <Adafruit_ILI9341.h>
#include <Adafruit_STMPE610.h>
#include "gui.h"

// The STMPE610 uses hardware SPI on the shield, and #8
#define STMPE_CS 8
Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

GUIService gui;

void GUIService::begin()
{
  tft.begin();

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(1);
  tft.println("Hello World!");

  if (!ts.begin()) {
    tft.println("Couldn't start touchscreen controller");
  }

  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
}
