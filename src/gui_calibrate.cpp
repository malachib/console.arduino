#include <SPI.h> // stokes auto-lib resolver for platformio
#include <fact/lib.h>
#include <Console.h>

#include "gui.h"
#include "monitor.h"
#include "states.h"

#include "main.h"



void SendVerticalScrollStartAddress(uint16_t wVSP);
/*
static union
{
  calibration::CalibrationState calibrationState;
  active::ActiveState activeState;
};
*/


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

struct InitializingState
{
  uint32_t timeInitialized;
  uint8_t lastDrawnSecond;
};

static union
{
  InitializingState initializingState;
};

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
      tft.setCursor(0, ROW_HEIGHT * 10);
      tft.setTextSize(2);
      tft << F("Touch to calibrate");
      if(eeprom.hasProfile())
      {
        tft.println();
        tft << F("Starting in ");
      }
      
      initializingState.timeInitialized = millis();

      break;

    case calibration::UpperLeftWaiting:
      if(eeprom.hasProfile())
      {
        // TODO: revamp and use old state machine framework code, perfect
        // use case right here
        // render second countdown here, rather inefficiently I might add
        uint32_t target = initializingState.timeInitialized + 4000;
        uint32_t current = millis();
        if(current > target)
        {
          // time has elapsed, use EEPROM stored value
          subState.calibration = calibration::Calibrated;
        }
        else
        {
          uint32_t remaining = target - current;
          auto remainingSeconds = 1+(remaining / 1000);
          if(remainingSeconds != initializingState.lastDrawnSecond)
          {
            tft.setCursor(14 * COLUMN_WIDTH, ROW_HEIGHT * 11);
            // erase previous character
            tft.fillRect(14 * COLUMN_WIDTH, ROW_HEIGHT * 11, COLUMN_WIDTH, ROW_HEIGHT, 0);
            tft.print(remainingSeconds);
            initializingState.lastDrawnSecond = remainingSeconds;
          }
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
