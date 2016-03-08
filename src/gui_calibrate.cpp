#include <SPI.h> // stokes auto-lib resolver for platformio
#include <fact/lib.h>
#include <Console.h>

#include "gui.h"
#include "monitor.h"
#include "states.h"

#include "main.h"



class GenericInterval : public util::IScheduledWithInterval<uint16_t>
{
  void (*f)();
  
public:
  GenericInterval(void (*f)()) : f(f) {}
  
  void setInterval(uint16_t i) { wakeupInterval = i; }
};

class Countdown : public util::IScheduledWithInterval<uint16_t>
{
  uint8_t secondsLeft = 4;
  
public:
  Countdown() : IScheduledWithInterval(1000) {  }
  
  uint8_t getSecondsLeft() { return secondsLeft; }
  
  void execute() override
  {
    // don't draw 0, because screen is just gonna immediately clear
    if(--secondsLeft == 0) return;
    
    tft.setCursor(14 * COLUMN_WIDTH, ROW_HEIGHT * 12);
    // erase previous character
    tft.fillRect(14 * COLUMN_WIDTH, ROW_HEIGHT * 12, COLUMN_WIDTH, ROW_HEIGHT, 0);
    tft.print(secondsLeft);
  }
};




#define CAL_EDGE_OFFSET 20
#define CAL_CIRCLE_RADIUS 25



void calibrationTouchResponder(TouchService* _touch)
{
  switch(subState.calibration)
  {
    case calibration::UpperLeftWaiting:
      touch.screenBounds.origin = _touch->lastPoint;
      subState.calibration = calibration::UpperLeftPressed;
      break;

    case calibration::LowerRightWaiting:
      touch.screenBounds.size = _touch->lastPoint - touch.screenBounds.origin;
      subState.calibration = calibration::LowerRightPressed;
      break;
  }
}

Countdown countdown;

void GUIService::stateHandlerCalibration()
{
  switch(subState.calibration)
  {
    case calibration::Initialize:
#ifdef DEBUG
      cout << F("Initialize Calibration");
      cout.println();
#endif
      touch.released.clear();
      touch.released += calibrationTouchResponder;
      tft.drawCircle(CAL_EDGE_OFFSET,CAL_EDGE_OFFSET,CAL_CIRCLE_RADIUS,0xFFFF);
      subState.calibration = calibration::UpperLeftWaiting;

      tft.setTextColor(ILI9341_WHITE);
      tft.setCursor(0, ROW_HEIGHT * 8);
      tft.setTextSize(2);
      tft << F("Touch to calibrate");
      if(eeprom.hasProfile())
      {
        tft.println();
        tft.println();
        tft << F("or wait for default");
        tft.println();
        tft << F("Starting in ");

        sm.add(countdown);
      }
      
      break;

    case calibration::UpperLeftWaiting:
      if(eeprom.hasProfile())
      {
        if(countdown.getSecondsLeft() == 0)
        {
          sm.remove(countdown);
          subState.calibration = calibration::Calibrated;
        }
      }
      break;
      
    case calibration::UpperLeftPressed:
      // erase upper left circle
      tft.fillRect(0,0,CAL_CIRCLE_RADIUS * 3,CAL_CIRCLE_RADIUS * 3,0);
      // draw lower right circle
      tft.drawCircle(SCREEN_WIDTH - CAL_EDGE_OFFSET,SCREEN_HEIGHT - CAL_EDGE_OFFSET,CAL_CIRCLE_RADIUS,0xFFFF);
      subState.calibration = calibration::LowerRightWaiting;
      break;
      
    case calibration::LowerRightPressed:
      subState.calibration = calibration::Calibrated;
      break;

    case calibration::Middle:
      break;

    case calibration::LowerRightWaiting:
      break;

    case calibration::Calibrated:
#ifdef DEBUG
      cout << F("Calibrated OR=") << touch.screenBounds.origin;
      cout << F(" and SZ=") << touch.screenBounds.size;
      cout.println();
#endif
      touch.calibrated = true;
      touch.released.clear();
      state = Active;
      subState.active = active::Initialize;
      
      if(!eeprom.hasProfile())
        eeprom.save();
        
      break;
  }
}
