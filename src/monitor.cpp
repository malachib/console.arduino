#include <Arduino.h>
#include "monitor.h"

MonitorService monitor;

Stream* MonitorService::stream;

void MonitorService::begin()
{
#if SAMD_SERIES
  stream = &Serial1;
#else
  stream = &Serial;
#endif
}
