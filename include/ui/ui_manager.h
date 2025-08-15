#pragma once
#include <Adafruit_SH110X.h>
#include "ui_view.h"
#include "../input_manager.h"

class UIManager {
public:
  UIManager(InputManager& input);
  bool begin();
  void nextView();
  void previousView();
  void update();
  void handleClick(); // Handle clicks for current view
  void handleRotate(int direction); // Handle rotation with sub-view delegation

private:
  Adafruit_SH1106G display;
  InputManager& inputManager;
  static const int MAX_VIEWS = 4;
  UIView* views[MAX_VIEWS];
  int viewCount = 0;
  int currentIndex = 0;
  void applyInputHandlers(); // Legacy method, no longer used
};
