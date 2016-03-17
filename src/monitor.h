#pragma once

template <class TStream>
class MonitorService
{
protected:
  static TStream* stream;
public:
  static void setBPS(uint32_t bps)
  {
    stream->begin(bps);
  }
  
  static TStream* getStream() { return stream; }
};

class SerialMonitorService :
  public MonitorService<HardwareSerial>
{
public:
  static void begin();
};

extern SerialMonitorService monitor;
