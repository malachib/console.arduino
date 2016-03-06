#include "gui.h"
#include "gfx_touch.h"

void TouchActionResponder::touch(_Vector3D<uint16_t> vector)
{
  Region* region = regionResponder.find(vector.x, vector.y);
}
