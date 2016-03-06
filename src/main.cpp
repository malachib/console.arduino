#include <Arduino.h>
#include "gui.h"
#include "monitor.h"

void setup()
{
  monitor.begin();
  gui.begin();
}

void loop()
{
  gui.stateHandler();
}
