#pragma once

class MonitorService
{
public:
  static void begin();

  static Stream* stream;
};

extern MonitorService monitor;
