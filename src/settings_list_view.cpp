#include "ui/views/settings_list_view.h"
#include "ui/views/pid_config_view.h"

SettingsListView::SettingsListView() {
  // Initialize menu items
  menuItems = {
    "PID Tuning",
    "WiFi Settings",
    "Control Options",
    "< Back"
  };
}

SettingsListView::~SettingsListView() {
  delete pidConfigView;
}

void SettingsListView::render(Adafruit_SH1106G* display) {
  // Check for sub-view first
  if (hasSubView()) {
    getSubView()->render(display);
    return;
  }
  
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SH110X_WHITE);
  
  // Title
  display->setCursor(0, 0);
  display->print("Settings Menu");
  display->drawLine(0, 10, 128, 10, SH110X_WHITE);
  
  // Menu items
  int y = 16;
  for (int i = 0; i < MAX_VISIBLE_ITEMS && (i + scrollOffset) < menuItems.size(); i++) {
    int itemIndex = i + scrollOffset;
    
    // Draw cursor
    if (itemIndex == currentCursor) {
      display->setCursor(0, y);
      display->print(">");
    }
    
    // Draw menu item
    display->setCursor(8, y);
    display->print(menuItems[itemIndex].c_str());
    
    y += 8;
  }
  
  // Scroll indicators
  if (scrollOffset > 0) {
    display->setCursor(120, 16);
    display->print("^");
  }
  if (scrollOffset + MAX_VISIBLE_ITEMS < menuItems.size()) {
    display->setCursor(120, 56);
    display->print("v");
  }
  
  display->display();
}

std::function<void(int)> SettingsListView::onRotate() {
  return [this](int dir) { handleRotate(dir); };
}

std::function<void()> SettingsListView::onClick() {
  return [this]() { handleClick(); };
}

void SettingsListView::handleRotate(int dir) {
  // If sub-view is active, delegate to it
  if (hasSubView()) {
    auto subViewRotate = getSubView()->onRotate();
    if (subViewRotate) {
      subViewRotate(dir);
    }
    return;
  }
  
  if (dir > 0) {
    // Rotate clockwise - move cursor down
    if (currentCursor < menuItems.size() - 1) {
      currentCursor++;
      updateScrollOffset();
    }
  } else {
    // Rotate counter-clockwise - move cursor up
    if (currentCursor > 0) {
      currentCursor--;
      updateScrollOffset();
    }
  }
}

void SettingsListView::handleClick() {
  // If sub-view is active, delegate to it
  if (hasSubView()) {
    auto subViewClick = getSubView()->onClick();
    if (subViewClick) {
      subViewClick();
    }
    return;
  }
  
  // Check if "Back" option is selected
  if (currentCursor == menuItems.size() - 1) {
    if (onBackCallback) {
      onBackCallback();
    }
  } else {
    // Handle menu item selection
    switch (currentCursor) {
      case 0:  // PID Tuning
        enterPidConfig();
        break;
      case 1:  // WiFi Settings
        // TODO: Implement WiFi settings
        break;
      case 2:  // Control Options
        // TODO: Implement control options
        break;
    }
  }
}

void SettingsListView::updateScrollOffset() {
  // Keep cursor visible within the display area
  if (currentCursor < scrollOffset) {
    scrollOffset = currentCursor;
  } else if (currentCursor >= scrollOffset + MAX_VISIBLE_ITEMS) {
    scrollOffset = currentCursor - MAX_VISIBLE_ITEMS + 1;
  }
}

void SettingsListView::enterPidConfig() {
  if (!pidConfigView) {
    pidConfigView = new PidConfigView();
    
    // Set up callback for returning to settings menu
    pidConfigView->setOnBackCallback([this]() {
      exitPidConfig();
    });
  }
  
  setSubView(pidConfigView);  // Activate sub-view
}

void SettingsListView::exitPidConfig() {
  clearSubView();  // Deactivate sub-view
}