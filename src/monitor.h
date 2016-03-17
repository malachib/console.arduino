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
  static void setBPS(uint32_t bps)
  {
    stream->begin(bps);
  }
};

extern SerialMonitorService monitor;
