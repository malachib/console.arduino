#include <menu.h>
#include <menuGFX.h>
#include <chainStream.h>// concatenate multiple input streams (this allows adding a button to the encoder)
#include <menuFields.h>

#include "gui.h"
#include "gfx_touch.h"

menuGFX gfx(tft);

void nothing() {}


Region regionUp(1, 0,0,240,30);
Region regionDown(2, 0,290,240,30);
Region regionEnter(0, 0,31,240,320 - 60);
//ActionRegion regionBackup(3,)
Region *actionRegions[] =
{ &regionUp, &regionDown, &regionEnter };

RegionResponder regionResponder(actionRegions);

// gather touch events as a stream, since this is
// how neu-rah's menu handler likes it
class TouchMenuStream : public Stream
{
  TouchResponder& touchResponder;
public:
  TouchMenuStream(TouchResponder& touchResponder) : touchResponder(touchResponder)
  {

  }

  // only call this when touch threshold has been reached, determined externally
  void touch(_Vector3D<uint16_t> vector);
};


MENU(subMenu,"Sub-Menu",
  OP("Op1",nothing),
  OP("Op2",nothing),
  OP("Op3",nothing)
);

MENU(mainMenu,"Sistema",
  OP("A",nothing),
  OP("B",nothing),
  SUBMENU(subMenu)
);

void MenuService::begin()
{

}


void MenuService::stateHandler()
{
  //mainMenu.poll(gfx);
}
