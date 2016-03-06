#include <Arduino.h>
#include "monitor.h"

MonitorService monitor;

Stream* MonitorService::stream;

void MonitorService::begin()
{
  stream = &Serial;
}
