#include <Arduino.h>
#include "gui.h"
#include "monitor.h"
#include "main.h"
#include <Scheduler.h>


void uiSetup()
{
  
}


void uiLoop()
{
  
}


void setup()
{

  eeprom.begin();
  //Scheduler.start(gui.begin, gui.stateHandler);
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
