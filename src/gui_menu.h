class MenuService
{
public:
  static void begin();
  static void stateHandler();
  static void touchReleasedHandler(TouchService* ts);
  static void touchTouchingHandler(TouchService* ts);
};

extern volatile bool inMenu;
