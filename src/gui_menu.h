class MenuService
{
public:
  static void begin();
  static bool stateHandler();
  static void touchReleasedHandler(TouchService* ts);
  static void touchTouchingHandler(TouchService* ts);
};

extern volatile bool inMenu;
