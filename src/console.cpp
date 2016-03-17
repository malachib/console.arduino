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

void gfx_diagnostic_scroll1(int dummyParam);
void gfx_diagnostic_scroll2();


CREATE_MENUFUNCTION(consoleSetBPS, setBPS, "Change BPS for target device");
CREATE_MENUFUNCTION(consoleDiagnostic1, gfx_diagnostic_scroll1, "Diagnostic for scroll behavior");
//CREATE_MENUFUNCTION(consoleDiagnostic2, gfx_diagnostic_scroll2, "Diagnostic for scroll behavior");

ConsoleService::MainMenu::MainMenu()
{
  add(consoleSetBPS);
  add(consoleDiagnostic1);
}
#endif
