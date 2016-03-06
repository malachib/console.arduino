#include <menu.h>
#include <menuGFX.h>
#include <chainStream.h>// concatenate multiple input streams (this allows adding a button to the encoder)
#include <menuFields.h>

#include "gui.h"

menuGFX gfx(tft);

void nothing() {}

// surprising these grid primitive aren't already in any of our GFX libs
// OK the touch screen lib does have its own TSPoint
template <class TPrecision>
struct _Vector
{
  TPrecision  x;
  TPrecision  y;
};

template <class TPrecision>
struct _Rectangle
{
protected:
  // where within drawing area this rectangle begins
  _Vector<TPrecision>  origin;
  // horizontal and vertical size of rectangle
  _Vector<TPrecision>  size;

public:
  TPrecision getX() { return origin.x; }
  TPrecision getY() { return origin.y; }
  TPrecision getWidth() { return size.x; }
  TPrecision getHeight() { return size.y; }
  TPrecision getBoundX() { return origin.x + size.x; }
  TPrecision getBoundY() { return origin.y + size.y; }
};

struct Rectangle : public _Rectangle<uint16_t>
{
  Rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
  {
    origin.x = x;
    origin.y = y;
    size.x = width;
    size.y = height;
  }
};

struct ActionRegion : public Rectangle
{
  uint8_t actionCode;

  ActionRegion(uint8_t actionCode, uint16_t x, uint16_t y, uint16_t width, uint16_t height) :
    Rectangle(x, y, width, height), actionCode(actionCode) {}
};


// gather touch events as a stream, since this is
// how neu-rah's menu handler likes it
class TouchMenuStream : public Stream
{
public:

};

class TouchResponder
{
  ActionRegion** regions;

public:
  TouchResponder(ActionRegion** regions) : regions(regions)
  {

  }

  ActionRegion* find(uint16_t x, uint16_t y)
  {
    ActionRegion** region = regions;

    for(int i = 3; i-- > 0;)
    {
      auto r = *region;
      if(x >= r->getX() && x <= r->getBoundX() &&
        y >= r->getY() && y <= r->getBoundY())
      {
        return r;
      }
    }

    return NULL;
  }
};

ActionRegion regionUp(1, 0,0,240,30);
ActionRegion regionDown(2, 0,290,240,30);
ActionRegion regionEnter(0, 0,31,240,320 - 60);
//ActionRegion regionBackup(3,)
ActionRegion *actionRegions[] =
{ &regionUp, &regionDown, &regionEnter };

TouchResponder touchResponder(actionRegions);

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
