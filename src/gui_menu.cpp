#include <menu.h>
#include <menuGFX.h>
#include <chainStream.h>// concatenate multiple input streams (this allows adding a button to the encoder)
#include <menuFields.h>
#include <genericKeyboard.h>

#include "gui.h"
#include "gfx_touch.h"

#include <fact/lib.h>

MenuService _menu;
menuGFX gfx(tft);

void nothing() {}

//TouchActionResponder
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
  void touch(Vector vector);
};


void TouchMenuStream::touch(Vector vector)
{
  //touchResponder
}

uint32_t bps;

void setBPS(prompt& p)
{
#ifdef DEBUG
  Serial << F("BPS set: ") << bps;
#endif
}

/*
MENU(subMenu2,"Sub-Menu",
  OP("Op1",nothing),
  OP("Op2",nothing),
  OP("Op3",nothing)
);
*/


CHOOSE(bps, subMenuBPS,"BPS: ",
  VALUE("2400",2400,setBPS),
  VALUE("4800",4800,setBPS),
  VALUE("9600",9600,setBPS),
  VALUE("19200",19200,setBPS),
  VALUE("57600",57600,setBPS),
  VALUE("115200",115200,setBPS)
);

MENU(mainMenu,"Sistema",
  OP("A",nothing),
  //OP("B",nothing),
  SUBMENU(subMenuBPS)
);

void MenuService::begin()
{
  gfx.resX*=2;//update resolution after font size change
  gfx.resY*=2;//update resolution after font size change

  gfx.maxX=12;
  gfx.maxY=4;
  gfx.bgColor=SILVER;

  gfx.top = 2;
}

int menuCommand;

void MenuService::touchReleasedHandler(TouchService *ts)
{
  Region* region = ts->lastPressed;
  uint8_t regionCode = region->regionCode;

  switch(regionCode)
  {
    // FIX: somehow my up/down codes are getting reversed
    // up
    case 2:
      menuCommand = menu::upCode;
      break;
    // down
    case 1:
      menuCommand = menu::downCode;
      break;
    // enter
    case 0:
      menuCommand = menu::enterCode;
      break;
  }
}

int getMenuCommand()
{
  int temp = menuCommand;
  menuCommand = -1;
  return temp;
}

genericKeyboard forcedKeyIn(getMenuCommand);


void MenuService::stateHandler()
{
  mainMenu.poll(gfx, forcedKeyIn);
}
