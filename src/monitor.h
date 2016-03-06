#pragma once

template <class TStream>
class MonitorService
{
public:
  static TStream* stream;
};

class SerialMonitorService :
  public MonitorService<HardwareSerial>
{
public:
  static void begin();
};

extern SerialMonitorService monitor;
