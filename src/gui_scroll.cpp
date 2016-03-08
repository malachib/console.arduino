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
