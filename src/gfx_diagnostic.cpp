#include "gui.h"

#include <fact/lib.h>

using namespace util;

void doCharScroll();

void gfx_diagnostic_scroll1(int dummyParam = 0)
{
  tft.fillScreen(ILI9341_BLACK);

  tft.setScrollStart(0);
  for(int i = 0; i < ROWS; i++)
  {
    tft << F("Debug line # ") << i;
    tft.println();
  }
}

void gfx_diagnostic_scroll2()
{
  
}
