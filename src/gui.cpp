#include <SPI.h> // stokes auto-lib resolver for platformio

#include "variants.h"
#include "gui.h"
#include "monitor.h"
#include <fact/lib.h>

#include <Adafruit_ILI9341.h>
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

// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

GUIService gui;

#ifdef DEBUG
#define ROW_HEIGHT 16
#else
#define ROW_HEIGHT 8
#endif
#define ROWS (320 / ROW_HEIGHT)

uint16_t iScrollStart = 8;

void SendVerticalScrollStartAddress(uint16_t wVSP);

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

  SendVerticalScrollStartAddress(0);

  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
#ifdef DEBUG
  tft.setTextSize(2);
  tft << F("Available rows:") << ROWS;
  tft.println();
#else
  tft.setTextSize(1);
  tft.println(F("Ready to listen"));
#endif

#ifndef ANALOG_TOUCHSCREEN
  if (!ts.begin()) {
    tft.println("Couldn't start touchscreen controller");
  }
#endif
}

// Using technique described here:
// https://forums.adafruit.com/viewtopic.php?f=47&t=56039&sid=d490b4c0369e06d31d4d4f18727cb152
void ScrollScreen(uint16_t px);

void GUIService::stateHandler()
{
  static bool scrollMode = false;

  while(monitor.stream->available())
  {
    int ch = monitor.stream->read();
    if(ch == 13)
    {
      uint8_t cursor_y = (tft.getCursorY() / ROW_HEIGHT); // we'll operate in rows, not pixels

#ifdef DEBUG
      Serial << F("cursor_y = ") << cursor_y;
      Serial << F("scrollMode = ") << scrollMode;
      Serial.println();
#endif

      if(cursor_y >= (ROWS - 1))
      {
        tft.setCursor(0, 0);
        cursor_y = 0;
        scrollMode = true;
      }

      if(scrollMode)
      {
        ScrollScreen(ROW_HEIGHT);
        // hardware scroll moves the actual screen origin around,
        // so we have to move the fillrect here to keep it stationary (on the bottom)
        // on the actual screen
        //tft.fillRect(0, 319 - ROW_HEIGHT, 239, 319, 0xFFFF);
        tft.fillRect(0, cursor_y * ROW_HEIGHT, 240, ROW_HEIGHT, 0);
      }

      tft.println();
    }
    else
      tft.print((char)ch);
  }
}


void ScrollScreen(uint16_t px) {
  // Try simple...
  iScrollStart += px;
  if (iScrollStart == 320)
    iScrollStart = px;
  SendVerticalScrollStartAddress(iScrollStart);
}

void WriteData16(uint16_t w) {
  tft.writedata(w >> 8);
  tft.writedata(w & 0xFF);     // XSTART

}

void SendVerticalScrollDefinition(uint16_t wTFA, uint16_t wBFA) {
  // Did not pass in VSA as TFA+VSA=BFA must equal 320
  tft.writecommand(0x33); // Vertical Scroll definition.
  WriteData16(wTFA);   //
  WriteData16(320-(wTFA+wBFA));
  WriteData16(wBFA);
}

#define ILI9340_CASET ILI9341_CASET
#define ILI9340_PASET ILI9341_PASET
#define ILI9340_RAMWR ILI9341_RAMWR

void SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

  tft.writecommand(ILI9340_CASET); // Column addr set
  WriteData16(x0);   // XSTART
  WriteData16(x1);   // XEND
  tft.writecommand(ILI9340_PASET); // Row addr set
  WriteData16(y0);   // YSTART
  WriteData16(y1);   // YEND
  tft.writecommand(ILI9340_RAMWR); // write to RAM
}

void SendVerticalScrollStartAddress(uint16_t wVSP) {
  tft.writecommand(0x37); // Vertical Scroll definition.
  WriteData16(wVSP);   //
}
