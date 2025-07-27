#include "ui/views/settings_view.h"
#include "ui/views/settings_list_view.h"

SettingsView::SettingsView() {
  // Constructor
}

SettingsView::~SettingsView() {
  delete settingsListView;
}

void SettingsView::render(Adafruit_SSD1306* display) {
  // If we have a sub-view, render it instead
  if (hasSubView()) {
    getSubView()->render(display);
    return;
  }
  
  // Render main settings view
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  // Centered gear icon (128x64 display, icon is 32x32)
  int x = (128 - 32) / 2;
  int y = 8;
  display->drawBitmap(x, y, gear_bitmap_32, 32, 32, SSD1306_WHITE);
  const char* text = "Settings";
  int text_width = strlen(text) * 6; // 6 pixels per character
  int text_x = (128 - text_width) / 2;
  int text_y = y + 32 + 4; // 4 pixels below the icon

  display->setCursor(text_x, text_y);
  display->print(text);
  
  // Show "Click to enter" hint
  display->setCursor(text_x - 12, text_y + 12);
  display->print("Click to enter");
  
  display->display();
}

std::function<void(int)> SettingsView::onRotate() {
  return [this](int dir) { handleRotate(dir); };
}

std::function<void()> SettingsView::onClick() {
  return [this]() { handleClick(); };
}

void SettingsView::handleRotate(int dir) {
  // If we have a sub-view, delegate to it
  if (hasSubView()) {
    auto subViewRotate = getSubView()->onRotate();
    if (subViewRotate) {
      subViewRotate(dir);
    }
  }
}

void SettingsView::handleClick() {
  // If we have a sub-view, delegate to it
  if (hasSubView()) {
    auto subViewClick = getSubView()->onClick();
    if (subViewClick) {
      subViewClick();
    }
  } else {
    // Enter settings menu
    enterSettingsMenu();
  }
}

void SettingsView::enterSettingsMenu() {
  if (!settingsListView) {
    settingsListView = new SettingsListView();
    
    // Set up callback for returning to main settings view
    settingsListView->setOnBackCallback([this]() {
      exitSettingsMenu();
    });
  }
  
  setSubView(settingsListView);
}

void SettingsView::exitSettingsMenu() {
  clearSubView();
}

void SettingsView::handleSettingsSelection(int index) {
  // Handle different settings options
  switch (index) {
    case 0: // Target Temp
      // TODO: Implement target temperature setting
      break;
    case 1: // Fan Speed
      // TODO: Implement fan speed setting
      break;
    case 2: // Damper Control
      // TODO: Implement damper control setting
      break;
    case 3: // Alarm Settings
      // TODO: Implement alarm settings
      break;
    case 4: // Display Brightness
      // TODO: Implement display brightness setting
      break;
    case 5: // WiFi Settings
      // TODO: Implement WiFi settings
      break;
    case 6: // Calibration
      // TODO: Implement calibration
      break;
    case 7: // Factory Reset
      // TODO: Implement factory reset
      break;
    default:
      break;
  }
}