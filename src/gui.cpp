#include <SPI.h> // stokes auto-lib resolver for platformio
#include <taskmanager.h>
#include <fact/lib.h>
#include <Console.h>

#include "gui.h"
#include "monitor.h"
#include "states.h"

#include "main.h"



// The display also uses hardware SPI, plus #9 & #10
#define TFT_CS 10
#define TFT_DC 9
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);

GUIService gui;

uint16_t iScrollStart = 0;

GUIService::State GUIService::state;

SubState subState;

void GUIService::begin()
{
#ifdef DEBUG
  cout << F("GUIService begin");
  cout.println();
#endif
#ifndef ADAFRUIT_ILI9341
  // chinese clone version needs LCD explitly turned on, although
  // we could probably tie the line high - WITH A RESISTOR - if I understand my
  // electronics right
  pinMode(7, OUTPUT);
  digitalWrite(7, HIGH);
#endif

  tft.begin();
  tft.fillScreen(ILI9341_BLACK);

  tft.setScrollStart(0);

#ifndef ANALOG_TOUCHSCREEN
  if (!ts.begin()) {
    tft.println("Couldn't start touchscreen controller");
  }
#endif
#ifdef DEBUG2
  cout.println("GUIService end");
#endif
}

Region regionUp(1, 0,0,240,60);
Region regionDown(2, 0,260,240,60);
Region regionEnter(0, 0,31,240,320 - 120);

Region *actionRegions[] =
{ &regionUp, &regionDown, &regionEnter };

RegionResponder regionResponder(actionRegions, 3);


void menuResponder(TouchService* ts)
{
#ifdef DEBUG
  cout << F("Menu activating");
  cout.println();
#endif

  subState.active = active::MenuInitialize;
}


void GUIService::initializeActive()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
#ifdef DEBUG
  tft << F("Available rows:") << ROWS;
  tft.println();
#else
  tft.println(F("Listening"));
#endif

  // we're gonna handle this ourselves so that we can scroll properly
  tft.setTextWrap(false);
  touch.released += menuResponder;
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


void GUIService::stateHandler()
{
  // make our schedule manager GUI "thread" specific, for now
  sm.execute();

  switch(state)
  {
    case Initializing:
#ifdef DEBUG
      cout.println("GUIService initializing");
#endif
      state = Calibration;
      break;

    case Active:
      switch(subState.active)
      {
        case active::Initialize:
          initializeActive();
          subState.active = active::MonitoringInitialize;
          break;

        case active::MonitoringInitialize:
          subState.active = active::Monitoring;
          break;

        case active::Monitoring:
          stateHandlerMonitor();
          break;

        case active::MenuInitialize:
          touch.reset(&regionResponder);
          touch.touching += MenuService::touchTouchingHandler;
          touch.released += MenuService::touchReleasedHandler;
          _menu.begin();
          subState.active = active::Menu;
          // TODO: need to have exit menu handler to remove/clear event handlers
          break;

        case active::Menu:
          // FIX: don't know how to detect when we're done with the menu
          _menu.stateHandler();
          //subState.active = active::MonitoringInitialize;
          break;
      }
      break;

    case Calibration:
      stateHandlerCalibration();
      break;
  }
  yield();
}


void GUIService::stateHandlerMonitor()
{
  while(monitor.getStream()->available())
  {
    int ch = monitor.getStream()->read();
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
  tft.setScrollStart(iScrollStart);
}
