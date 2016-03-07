#include <Arduino.h>
#include "gui.h"
#include "monitor.h"
//#include <Scheduler.h>

Region regionUp(1, 0,0,240,30);
Region regionDown(2, 0,290,240,30);
Region regionEnter(0, 0,31,240,320 - 60);

Region regionMain(0,0,0,240,320);

Region *actionRegions[] =
{ &regionUp, &regionDown, &regionEnter };

Region *mainScreenRegions[] =
{ &regionMain };

RegionResponder regionResponder(actionRegions, 3);
RegionResponder mainRegionResponder(mainScreenRegions, 1);

bool menuMode = false;

void doMenus(TouchService* ts)
{
  //ts->lastPressed
  // last pressed doesn't matter here since the whole screen has one response
  // to touch

  // NOTE: keep an eye on this, 1) clear is a new call and 2) untested how
  // it will perform in the middle of its own event responder (should be OK
  // as long as we don't add NEW event handlers during an event handle, but
  // that's probably *exactly* what we want to do...) so looks like we may
  // be getting into garbage collection next --
  // or, instead, have two different TouchService objects.  If we go that
  // route, we can potentially do away with events entirely... although
  // they are a nice feature to have
  //ts->begin(&regionResponder);
  //menuMode = true;
}

void setup()
{
  gui.begin();
  monitor.begin();
  //touch.begin(&regionResponder);
  //touch.begin(&mainRegionResponder);
  touch.begin();
  //touch.released += doMenus;
  //Scheduler.start(NULL, eventHandler);
  _menu.begin();
}


void loop()
{
  touch.stateHandler();
  if(menuMode)
  {
    _menu.stateHandler();
  }
  else
    gui.stateHandler();
}
