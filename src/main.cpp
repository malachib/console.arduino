#include <Arduino.h>
#include "gui.h"
#include "monitor.h"
//#include <Scheduler.h>


void setup()
{
  gui.begin();
  monitor.begin();
  touch.begin();
  _menu.begin();
}


void loop()
{
  touch.stateHandler();
  gui.stateHandler();
}
