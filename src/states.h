
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
    Initialize,
    MonitoringInitialize,
    Monitoring,
    Menu,
    MenuInitialize
  };
}
