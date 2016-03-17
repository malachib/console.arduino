#include "gui.h"

#include <fact/lib.h>

using namespace util;

void doCharScroll();

void gfx_diagnostic_scroll1(int dummyParam = 0)
{
  tft.fillScreen(ILI9341_BLACK);

  tft.setScrollStart(0);
  tft.setCursor(0, 0);
  for(int i = 0; i < ROWS; i++)
  {
    tft << F("Debug line # ") << i;
    tft.println();
  }
  
  for(int j = 0; j < 5;j++)
  {
    for(int i = 0; i <ROWS; i++)
    {
      delay(50);
      tft.setScrollStart(i * ROW_HEIGHT);
    }
  }
  tft.setScrollStart(0);
  
  for( int i = 0 ; i < ROWS; i++)
  {
    delay(250);
    doCharScroll();
  }

  tft.setCursor(0,0);
  for(int i = 0; i < ROWS; i++)
  {
    tft << F("line # ") << i;
    tft.println();
  }

  for( int i = 0 ; i < ROWS; i++)
  {
    delay(250);
    tft << F("Debug2 line # ") << i;
    /// doCharScroll does println for us
    doCharScroll();
  }
}

void gfx_diagnostic_scroll2()
{
  
}
