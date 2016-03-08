#include <SPI.h> // stokes auto-lib resolver for platformio

#include "gui.h"
#include "monitor.h"
#include "states.h"
#include <fact/lib.h>



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
    case calibration::UpperLeft:
      touch.screenBounds.origin = _touch->lastPoint;
      subState.calibration = calibration::LowerRight;
#ifdef DEBUG
      Serial << F("UpperLeft Calibration");
      Serial.println();
#endif
      tft.fillScreen(ILI9341_BLACK);
      tft.drawCircle(SCREEN_WIDTH - CAL_EDGE_OFFSET,SCREEN_HEIGHT - CAL_EDGE_OFFSET,CAL_CIRCLE_RADIUS,0xFFFF);

      break;

    case calibration::LowerRight:
      touch.screenBounds.size = _touch->lastPoint - touch.screenBounds.origin;

#ifdef DEBUG2
      Serial << F("LowerRight Calibration: size (adj) = ") <<
        touchCalibration.screenBounds.size;
      Serial.println();
#endif

      subState.calibration = calibration::Calibrated;
#ifdef DEBUG2
      Serial << F("LowerLeft Calibration II");
      Serial.println();
#endif
      break;
  }
}

void GUIService::stateHandlerCalibration()
{
  switch(subState.calibration)
  {
    case calibration::Initialize:
#ifdef DEBUG
      Serial << F("Initialize Calibration");
      Serial.println();
#endif
      touch.released.clear();
      touch.released += calibrationTouchResponder;
      tft.drawCircle(CAL_EDGE_OFFSET,CAL_EDGE_OFFSET,CAL_CIRCLE_RADIUS,0xFFFF);
      subState.calibration = calibration::UpperLeft;
      break;

    case calibration::UpperLeft:
      break;

    case calibration::Middle:
      break;

    case calibration::LowerRight:
      break;

    case calibration::Calibrated:
#ifdef DEBUG
      Serial << F("Calibrated OR=") << touch.screenBounds.origin;
      Serial << F(" and SZ=") << touch.screenBounds.size;
      Serial.println();
#endif
      touch.calibrated = true;
      touch.released.clear();
      state = Active;
      subState.active = active::Initialize;
      break;
  }
}
