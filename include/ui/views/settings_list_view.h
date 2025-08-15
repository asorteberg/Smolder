#pragma once
#include "../ui_view.h"
#include <vector>
#include <string>

// Forward declaration
class PidConfigView;

class SettingsListView : public UIView {
public:
  SettingsListView();
  ~SettingsListView();
  void render(Adafruit_SH1106G* display) override;
  std::function<void(int)> onRotate() override;
  std::function<void()> onClick() override;
  
  // Navigation callbacks
  void setOnBackCallback(std::function<void()> callback) { onBackCallback = callback; }

private:
  std::vector<std::string> menuItems;
  int currentCursor = 0;
  int scrollOffset = 0;
  static const int MAX_VISIBLE_ITEMS = 6; // 6 lines fit on 64px display
  
  // Sub-view management
  PidConfigView* pidConfigView = nullptr;
  
  void handleRotate(int dir);
  void handleClick();
  void updateScrollOffset();
  void enterPidConfig();
  void exitPidConfig();
  
  std::function<void()> onBackCallback;
};