#include <Arduino.h>
#include <taskmanager.h>
#include "gui.h"
#include "monitor.h"
#include "main.h"
#include <Scheduler.h>
#include <Console.h>


util::ScheduleManager sm;


void setup()
{
#ifdef DEBUG
  cout.begin(115200);
#endif

  eeprom.begin();
#ifdef MULTITASK
  Scheduler.start(gui.begin, gui.stateHandler);
#else
  gui.begin();
#endif
  monitor.begin();
  touch.begin();
  _menu.begin();
  
  #ifdef DEBUG
    cout.println("initialized");
  #endif
}


void loop()
{
  touch.stateHandler();
#ifdef FEATURE_CONSOLE
  console.stateHandler();
#endif
#ifndef MULTITASK
  gui.stateHandler();
#else
  yield();
#endif
}
