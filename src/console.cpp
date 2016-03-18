#include "main.h"

#ifdef FEATURE_CONSOLE
#include <fact/MenuFunction.h>
#include "monitor.h"

using namespace util;

// useful since the touch UI isn't working quite right, and
// also just for diagnostics
int setBPS(uint32_t bps)
{
  monitor.setBPS(bps);
  return 1;
}

int dummyCommand(uint32_t value)
{
  return value;
}

void setBPS2(uint32_t bps)
{
  monitor.setBPS(bps);
}

ConsoleService* dummyService;

void menuDiagnostic()
{
  ConsoleService service2;
  
  dummyService = &service2;
}

void gfx_diagnostic_scroll1(int dummyParam);
void gfx_diagnostic_scroll2();


CREATE_MENUFUNCTION(consoleSetBPS, setBPS, "Change BPS for target device");
CREATE_MENUFUNCTION(consoleDiagnostic1, gfx_diagnostic_scroll1, "Diagnostic for scroll behavior");
//CREATE_MENUFUNCTION(consoleDiagnostic2, gfx_diagnostic_scroll2, "Diagnostic for scroll behavior");
//CREATE_MENUFUNCTION(consoleDiagnostic3, setBPS2, "Diagnostic for scroll behavior");
CREATE_MENUFUNCTION(consoleMenuDiagnostic, menuDiagnostic, "Menu Diagnostic");
CREATE_MENUFUNCTION(consoleDummyCommand, dummyCommand, "echoes back input #");

ConsoleService console;


// I'm thinking it's possible both static globals are initializing at the same time
// and next gets assigned to NULL *after* the add statement occurs?
// NOTE: yes above is true - moving console init below the menuFunctions cleared up
// the issue.  That is conrete fuel to move away from the constructor-oriented approach
// and towards a "begin" call instead
ConsoleService::MainMenu::MainMenu()
{
  add(consoleSetBPS);
  add(consoleMenuDiagnostic);
  add(consoleDiagnostic1);
  //add(consoleDiagnostic3);
  add(consoleDummyCommand);
}
#endif
