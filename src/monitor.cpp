#include <Arduino.h>
#include "monitor.h"

SerialMonitorService monitor;

template <class TStream>
TStream* MonitorService<TStream>::stream;

void SerialMonitorService::begin()
{
  #if SAMD_SERIES
    stream = &Serial1;
  #else
    stream = &Serial;
  #endif

  setBPS(115200);
}
