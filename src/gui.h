#pragma once

#include <Service.h>

class GUIService
{
public:
  GUIService() {}

  static void begin();
  static void stateHandler();
};

extern GUIService gui;
