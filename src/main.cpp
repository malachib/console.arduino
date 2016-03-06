#include <Arduino.h>
#include "gui.h"
#include "monitor.h"
//#include <Scheduler.h>

Region regionUp(1, 0,0,240,30);
Region regionDown(2, 0,290,240,30);
Region regionEnter(0, 0,31,240,320 - 60);
//ActionRegion regionBackup(3,)
Region *actionRegions[] =
{ &regionUp, &regionDown, &regionEnter };

RegionResponder regionResponder(actionRegions);


void setup()
{
  gui.begin();
  monitor.begin();
  touch.begin(&regionResponder);
  //Scheduler.start(NULL, eventHandler);
}

void loop()
{
  gui.stateHandler();
  touch.stateHandler();
}
