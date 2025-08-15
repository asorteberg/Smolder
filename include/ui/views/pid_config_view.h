#pragma once
#include "../ui_view.h"
#include "../../config.h"
#include <vector>
#include <string>

class PidConfigView : public UIView {
public:
  PidConfigView();
  void render(Adafruit_SH1106G* display) override;
  std::function<void(int)> onRotate() override;
  std::function<void()> onClick() override;
  
  // Navigation callbacks
  void setOnBackCallback(std::function<void()> callback) { onBackCallback = callback; }

private:
  enum EditState {
    MENU_NAVIGATION,
    EDITING_KP,
    EDITING_KI,
    EDITING_KD
  };
  
  EditState currentState = MENU_NAVIGATION;
  int currentCursor = 0;
  int scrollOffset = 0;
  static const int MAX_VISIBLE_ITEMS = 4; // 4 lines fit nicely with title
  
  void handleRotate(int dir);
  void handleClick();
  void updateScrollOffset();
  void enterEditMode(int paramIndex);
  void exitEditMode();
  std::string formatPidValue(float value) const;
  
  std::function<void()> onBackCallback;
};