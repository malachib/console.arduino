#include "gui.h"

uint16_t iScrollStart = 0;


void ScrollScreen(uint16_t px) {
  iScrollStart += px;
  if (iScrollStart == SCREEN_HEIGHT)
    iScrollStart = 0;
  tft.setScrollStart(iScrollStart);
}

void doCharScroll()
{
  static bool scrollMode = false;

  uint8_t cursor_y = (tft.getCursorY() / ROW_HEIGHT); // we'll operate in rows, not pixels

#ifdef DEBUG
  Serial << F("scrollMode = ") << scrollMode;
  Serial << F(" cursor_y = ") << cursor_y;
  Serial << F(" cursor_y [actual] = ") << tft.getCursorY();
  Serial << F(" scrollStart = ") << iScrollStart;
  Serial.println();
#endif

  if(cursor_y >= (ROWS - 1))
  {
    tft.setCursor(0, 0);
    cursor_y = 0;
    scrollMode = true;
  }
  else
  {
    tft.println();
    cursor_y++;
  }

  if(scrollMode)
  {
    // hardware scroll moves the actual screen origin around,
    // so we have to move the fillrect here to keep it stationary (on the bottom)
    // on the actual screen
    //tft.fillRect(0, 319 - ROW_HEIGHT, 239, 319, 0xFFFF);

    // erase next line
    tft.fillRect(0, cursor_y * ROW_HEIGHT, SCREEN_WIDTH, ROW_HEIGHT, 0);
    ScrollScreen(ROW_HEIGHT);
  }
}



#ifdef UNUSED
void SendVerticalScrollDefinition(uint16_t wTFA, uint16_t wBFA) {
  // Did not pass in VSA as TFA+VSA=BFA must equal 320
  tft.writecommand(0x33); // Vertical Scroll definition.
  tft.writedata16(wTFA);   //
  tft.writedata16(320-(wTFA+wBFA));
  tft.writedata16(wBFA);
}

#define ILI9340_CASET ILI9341_CASET
#define ILI9340_PASET ILI9341_PASET
#define ILI9340_RAMWR ILI9341_RAMWR

void SetAddressWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {

  tft.writecommand(ILI9340_CASET); // Column addr set
  tft.writedata16(x0);   // XSTART
  tft.writedata16(x1);   // XEND
  tft.writecommand(ILI9340_PASET); // Row addr set
  tft.writedata16(y0);   // YSTART
  tft.writedata16(y1);   // YEND
  tft.writecommand(ILI9340_RAMWR); // write to RAM
}
#endif
