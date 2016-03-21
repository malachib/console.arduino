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
          // return of false value means we are done with the menu
          if(!_menu.stateHandler())
            subState.active = active::MenuClose;
          break;
          
        case active::MenuClose:
          //touch.touching -= MenuService::touchTouchingHandler;
          //touch.released -= MenuService::touchReleasedHandler;
          tft.fillScreen(ILI9341_BLACK);
          tft.setCursor(0, 0);
          subState.active = active::MonitoringInitialize;
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
    // for now, totally eat LF and treat CR as CRLF, cuz
    // otherwise it ends up double-spacing.  Odd, cuz
    // tft.print on its own shouldn't scroll and 
    // cursor_x should only scroll ocassionally but somehow
    // it double-spaces all the time
    else if(ch == 10)
    {
      
    }
    else
    {
      // here we assume a printable character
      // FIX: Is this todo still relevant? TODO: next up is word wrap handling
      uint8_t cursor_x = (tft.getCursorX() / COLUMN_WIDTH);
      if(cursor_x == COLUMNS)
        doCharScroll();

      tft.print((char)ch);
    }
  }
}
