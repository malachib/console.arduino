#include "main.h"

#ifdef FEATURE_CONSOLE
#include <fact/MenuFunction.h>
#include "monitor.h"

ConsoleService console;

using namespace util;

// useful since the touch UI isn't working quite right, and
// also just for diagnostics
void setBPS(uint32_t bps)
{
  monitor.setBPS(bps);
}

CREATE_MENUFUNCTION(consoleSetBPS, setBPS, "Change BPS for target device");

ConsoleService::MainMenu::MainMenu()
{
  add(consoleSetBPS);
}
#endif
