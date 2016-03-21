#include <menu.h>
#include <menuGFX.h>
#include <chainStream.h>// concatenate multiple input streams (this allows adding a button to the encoder)
#include <menuFields.h>
#include <genericKeyboard.h>

#include <Console.h>

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

void _screenCalibration()
{
  //gui.state = GUIService::Calibration;
}


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
  OP("Screen Calibration",_screenCalibration),
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

volatile int menuCommand = -1;
volatile bool inMenu = false;
uint32_t lastReleasedTime;

void MenuService::touchTouchingHandler(TouchService* ts)
{
  // hold for 3 seconds to enter menu
  if(millis() > (ts->lastPressedTime + 3000))
  {
#ifdef DEBUG
    //cout.println("Long pressed");
#endif
    // switch into inMenu and generate one menuCommand.  
    // we don't want to keep reassigning menuCommand, because
    // that will enter the menu over and over
    if(!inMenu)
    {
  #ifdef DEBUG2
      cout.println("Menu activating II");
  #endif
      menuCommand = menu::enterCode;
      inMenu = true;
      // FIX: kludge to make sure stateHandler doesn't immediately abort since
      // the menu gets rendered before a release is registered
      lastReleasedTime = millis();
    }
  }
}

void MenuService::touchReleasedHandler(TouchService *ts)
{
  lastReleasedTime = millis();
  
  // only watch for keypresses if we've entered menu mode
  if(!inMenu) return;
  
  // if we have an escape code from a long press, then just exit
  if(menuCommand == menu::escCode) return;
  
  Region* region = ts->lastPressed;
  uint8_t regionCode = region->regionCode;
  
  switch(regionCode)
  {
    // FIX: somehow my up/down codes are getting reversed
    // up
    case 2:
      menuCommand = menu::upCode;
      //menuCommand = menu::escCode;
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
#ifdef DEBUG
  if(temp != -1)
  {
    cout << F("Touchscreen command: ") << temp;
    cout.println();
  }
#endif
  menuCommand = -1;
  return temp;
}

genericKeyboard forcedKeyIn(getMenuCommand);


bool MenuService::stateHandler()
{
  if(inMenu)
  {
    // if 7s go by and nothing was pressed, we auto-exit
    if(millis() > lastReleasedTime + 7000)
    {
      inMenu = false;
      return false;
    }
      
    mainMenu.poll(gfx, forcedKeyIn, true);
  }
  
  return true;
}
