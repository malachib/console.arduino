#include <SPI.h> // stokes auto-lib resolver for platformio

#include "gui.h"
#include "monitor.h"
#include <fact/lib.h>



// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

GUIService gui;

#ifdef DEBUG
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

uint16_t iScrollStart = 0;

void SendVerticalScrollStartAddress(uint16_t wVSP);

GUIService::State GUIService::state;

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

#ifndef ANALOG_TOUCHSCREEN
  if (!ts.begin()) {
    tft.println("Couldn't start touchscreen controller");
  }
#endif
}

void GUIService::initializeActive()
{
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

  // we're gonna handle this ourselves so that we can scroll properly
  tft.setTextWrap(false);
}

// Using technique described here:
// https://forums.adafruit.com/viewtopic.php?f=47&t=56039&sid=d490b4c0369e06d31d4d4f18727cb152
void ScrollScreen(uint16_t px);

void doCharScroll()
{
  static bool scrollMode = false;

  uint8_t cursor_y = (tft.getCursorY() / ROW_HEIGHT); // we'll operate in rows, not pixels

#ifdef DEBUG
  Serial << F("scrollMode = ") << scrollMode;
  Serial << F(" cursor_y = ") << cursor_y;
  Serial << F(" cursor_y [actual] = ") << tft.getCursorY();
  Serial << F(" scrollStart = ") << iScrollStart;
  Serial.println();
#endif

  if(cursor_y >= (ROWS - 1))
  {
    tft.setCursor(0, 0);
    cursor_y = 0;
    scrollMode = true;
  }
  else
  {
    tft.println();
    cursor_y++;
  }

  if(scrollMode)
  {
    // hardware scroll moves the actual screen origin around,
    // so we have to move the fillrect here to keep it stationary (on the bottom)
    // on the actual screen
    //tft.fillRect(0, 319 - ROW_HEIGHT, 239, 319, 0xFFFF);

    // erase next line
    tft.fillRect(0, cursor_y * ROW_HEIGHT, SCREEN_WIDTH, ROW_HEIGHT, 0);
    ScrollScreen(ROW_HEIGHT);
  }
}

void touch_test();

namespace calibration
{
  // TODO: try to get union to live more happily within state class.
  // anonymous static union made it unhappy
  enum CalibrationState
  {
    // we begin with a 2 second countdown to recalibrate screen if desired,
    // if screen is not touched then we proceed straight into monitor mode
    Initialize,
    UpperLeft,
    Middle,
    LowerRight,
    Calibrated
  };
}

namespace active
{
  enum ActiveState
  {
    Initialize, // yuck
    Monitoring
  };
}


static union
{
  calibration::CalibrationState calibrationState;
  active::ActiveState activeState;
};


void GUIService::stateHandler()
{
  switch(state)
  {
    case Initializing:
      state = Calibration;
      break;

    case Active:
      switch(activeState)
      {
        case active::Initialize:
          initializeActive();
          activeState = active::Monitoring;
          break;

        case active::Monitoring:
          stateHandlerMonitor();
          break;
      }
      break;

    case Calibration:
      stateHandlerCalibration();
      break;
  }
}

#define CAL_EDGE_OFFSET 10
#define CAL_CIRCLE_RADIUS 30



Vector TouchService::upperLeft;
Vector TouchService::lowerRight;

void calibrationTouchResponder(TouchService* touch)
{
  switch(calibrationState)
  {
    case calibration::UpperLeft:
      TouchService::upperLeft = touch->lastPoint;
      calibrationState = calibration::LowerRight;
#ifdef DEBUG
      Serial << F("UpperLeft Calibration");
      Serial.println();
#endif
      tft.fillScreen(ILI9341_BLACK);
      tft.drawCircle(SCREEN_WIDTH - CAL_EDGE_OFFSET,SCREEN_HEIGHT - CAL_EDGE_OFFSET,CAL_CIRCLE_RADIUS,0xFFFF);

      break;

    case calibration::LowerRight:
#ifdef DEBUG
      Serial << F("LowerRight Calibration");
      Serial.println();
#endif
      TouchService::lowerRight = touch->lastPoint;
      calibrationState = calibration::Calibrated;
#ifdef DEBUG2
      Serial << F("LowerLeft Calibration II");
      Serial.println();
#endif
      break;
  }
}

void GUIService::stateHandlerCalibration()
{
  switch(calibrationState)
  {
    case calibration::Initialize:
#ifdef DEBUG
      Serial << F("Initialize Calibration");
      Serial.println();
#endif
      touch.released.clear();
      touch.released += calibrationTouchResponder;
      tft.drawCircle(CAL_EDGE_OFFSET,CAL_EDGE_OFFSET,CAL_CIRCLE_RADIUS,0xFFFF);
      calibrationState = calibration::UpperLeft;
      break;

    case calibration::UpperLeft:
      break;

    case calibration::Middle:
      break;

    case calibration::LowerRight:
      break;

    case calibration::Calibrated:
#ifdef DEBUG
      Serial << F("Calibrated UL=") << touch.upperLeft << " and LR=" << touch.lowerRight;
      Serial.println();
#endif
      touch.calibrated = true;
      touch.released.clear();
      state = Active;
      activeState = active::Initialize;
      break;
  }
}

void GUIService::stateHandlerMonitor()
{
  while(monitor.stream->available())
  {
    int ch = monitor.stream->read();
    if(ch == 13)
    {
      doCharScroll();
    }
    else
    {
      // TODO: next up is word wrap handling
      uint8_t cursor_x = (tft.getCursorX() / COLUMN_WIDTH);
      if(cursor_x == COLUMNS)
        doCharScroll();

      tft.print((char)ch);
    }
  }
}


void ScrollScreen(uint16_t px) {
  // Try simple...
  iScrollStart += px;
  if (iScrollStart == SCREEN_HEIGHT)
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
