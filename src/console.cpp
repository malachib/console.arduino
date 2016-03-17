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

void setBPS2(uint32_t bps)
{
  monitor.setBPS(bps);
}

ConsoleService* dummyService;

void menuDiagnostic(int dummyParam)
{
  ConsoleService service2;
  
  dummyService = &service2;
}

void gfx_diagnostic_scroll1(int dummyParam);
void gfx_diagnostic_scroll2();


CREATE_MENUFUNCTION(consoleSetBPS, setBPS, "Change BPS for target device");
CREATE_MENUFUNCTION(consoleDiagnostic1, gfx_diagnostic_scroll1, "Diagnostic for scroll behavior");
//CREATE_MENUFUNCTION(consoleDiagnostic2, gfx_diagnostic_scroll2, "Diagnostic for scroll behavior");
CREATE_MENUFUNCTION(consoleDiagnostic3, setBPS2, "Diagnostic for scroll behavior");
CREATE_MENUFUNCTION(consoleMenuDiagnostic, menuDiagnostic, "Menu Diagnostic");

// I'm thinking it's possible both static globals are initializing at the same time
// and next gets assigned to NULL *after* the add statement occurs?
ConsoleService::MainMenu::MainMenu()
{
#ifdef DEBUG
  cout << F("MainMenu::menus count = ") << count();
  cout.println();
#endif
  //add(consoleSetBPS);
  add(consoleMenuDiagnostic);
#ifdef DEBUG
  cout << F("MainMenu::menus count = ") << count();
  cout.println();
#endif
  add(consoleDiagnostic1);
#ifdef DEBUG
  cout << F("MainMenu::menus count = ") << count();
  cout.println();
#endif
  add(consoleDiagnostic3);
#ifdef DEBUG
  cout << F("MainMenu::menus count = ") << count();
  cout.println();
#endif
}
#endif
