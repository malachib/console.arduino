#include <menu.h>
#include <menuGFX.h>
#include <chainStream.h>// concatenate multiple input streams (this allows adding a button to the encoder)
#include <menuFields.h>

#include "gui.h"
#include "gfx_touch.h"

menuGFX gfx(tft);

void nothing() {}


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
