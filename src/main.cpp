#include <Arduino.h>
#include "gui.h"
#include "monitor.h"
//#include <Scheduler.h>

void setup()
{
  gui.begin();
  monitor.begin();
  touch.begin();
  //Scheduler.start(NULL, eventHandler);
}

void loop()
{
  gui.stateHandler();
  touch.stateHandler();
}
