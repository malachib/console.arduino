#pragma once

namespace calibration
{
  // TODO: try to get union to live more happily within state class.
  // anonymous static union made it unhappy
  enum CalibrationState
  {
    // we begin with a 2 second countdown to recalibrate screen if desired,
    // if screen is not touched then we proceed straight into monitor mode
    Initialize,
    UpperLeftWaiting,
    UpperLeftPressed,
    Middle,
    LowerRightWaiting,
    LowerRightPressed,
    Calibrated
  };
}

namespace active
{
  enum ActiveState
  {
    Initialize,
    MonitoringInitialize,
    Monitoring,
    Menu,
    MenuInitialize
  };
}

namespace __menu
{
  enum MenuState
  {
    Initialize,
    Deactivate
  };
}

union SubState
{
  calibration::CalibrationState calibration;
  active::ActiveState active;
  __menu::MenuState menuState;
};
