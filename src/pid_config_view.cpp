#include "ui/views/pid_config_view.h"
#include <sstream>
#include <iomanip>
#include "config.h"

PidConfigView::PidConfigView() {
  // Constructor - no menu items needed as we render dynamically
}

void PidConfigView::render(Adafruit_SSD1306* display) {
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  
  // Title
  display->setCursor(0, 0);
  display->print("PID Tuning");
  display->drawLine(0, 10, 128, 10, SSD1306_WHITE);
  
  // PID parameters
  int y = 16;
  
  // Kp parameter
  if (currentCursor == 0) {
    display->setCursor(0, y);
    display->print(">");
  }
  display->setCursor(8, y);
  display->print("Kp: ");
  display->print(formatPidValue(kpValue).c_str());
  if (currentState == EDITING_KP) {
    display->print(" *");
  }
  y += 12;
  
  // Ki parameter
  if (currentCursor == 1) {
    display->setCursor(0, y);
    display->print(">");
  }
  display->setCursor(8, y);
  display->print("Ki: ");
  display->print(formatPidValue(kiValue).c_str());
  if (currentState == EDITING_KI) {
    display->print(" *");
  }
  y += 12;
  
  // Kd parameter
  if (currentCursor == 2) {
    display->setCursor(0, y);
    display->print(">");
  }
  display->setCursor(8, y);
  display->print("Kd: ");
  display->print(formatPidValue(kdValue).c_str());
  if (currentState == EDITING_KD) {
    display->print(" *");
  }
  y += 12;
  
  // Back option
  if (currentCursor == 3) {
    display->setCursor(0, y);
    display->print(">");
  }
  display->setCursor(8, y);
  display->print("< Back");
  
  // Status line
  if (currentState != MENU_NAVIGATION) {
    display->setCursor(0, 56);
    display->print("Click to save");
  }
  
  display->display();
}

std::function<void(int)> PidConfigView::onRotate() {
  return [this](int dir) { handleRotate(dir); };
}

std::function<void()> PidConfigView::onClick() {
  return [this]() { handleClick(); };
}

void PidConfigView::handleRotate(int dir) {
  if (currentState == MENU_NAVIGATION) {
    // Navigate menu
    if (dir > 0) {
      // Rotate clockwise - move cursor down
      if (currentCursor < 3) {  // 0-3 for Kp, Ki, Kd, Back
        currentCursor++;
      }
    } else {
      // Rotate counter-clockwise - move cursor up
      if (currentCursor > 0) {
        currentCursor--;
      }
    }
  } else {
    // Edit PID values
    float* valuePtr = nullptr;
    float increment = 0.1f;
    
    switch (currentState) {
      case EDITING_KP:
        valuePtr = &kpValue;
        break;
      case EDITING_KI:
        valuePtr = &kiValue;
        increment = 0.01f;  // Smaller increments for integral term
        break;
      case EDITING_KD:
        valuePtr = &kdValue;
        break;
      default:
        return;
    }
    
    if (valuePtr) {
      if (dir > 0) {
        *valuePtr += increment;
        if (*valuePtr > 100.0f) *valuePtr = 100.0f;  // Max limit
      } else {
        *valuePtr -= increment;
        if (*valuePtr < 0.0f) *valuePtr = 0.0f;  // Min limit
      }
    }
  }
}

void PidConfigView::handleClick() {
  if (currentState == MENU_NAVIGATION) {
    // Handle menu navigation
    if (currentCursor == 3) {  // Back option
      if (onBackCallback) {
        onBackCallback();
      }
    } else {
      // Enter edit mode for selected parameter
      enterEditMode(currentCursor);
    }
  } else {
    // Exit edit mode and save value
    exitEditMode();
  }
}

void PidConfigView::updateScrollOffset() {
  // Not needed for PID config view as all items fit on screen
}

void PidConfigView::enterEditMode(int paramIndex) {
  switch (paramIndex) {
    case 0:
      currentState = EDITING_KP;
      break;
    case 1:
      currentState = EDITING_KI;
      break;
    case 2:
      currentState = EDITING_KD;
      break;
  }
}

void PidConfigView::exitEditMode() {
  currentState = MENU_NAVIGATION;
  // Here you could save values to EEPROM or send to controller
}

std::string PidConfigView::formatPidValue(float value) const {
  std::ostringstream oss;
  oss << std::fixed << std::setprecision(2) << value;
  return oss.str();
}