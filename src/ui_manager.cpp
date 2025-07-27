#include "ui/ui_manager.h"
#include "ui/views/temp_view.h"
#include "ui/views/settings_view.h"
#include "ui/views/smolder_logo.h"

UIManager::UIManager(InputManager& input) 
  : display(128, 64, &Wire, -1), inputManager(input), viewCount(0), currentIndex(0) {
}

bool UIManager::begin() {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    return false;
  }

  display.clearDisplay();
  display.drawBitmap(32, 0, smolder_logo, 64, 128, SSD1306_WHITE);
  display.display();
  delay(2000); 
  
  // Initialize views
  views[0] = new TempView();
  views[1] = new SettingsView();
  viewCount = 2;
  
  display.clearDisplay();
  views[currentIndex]->render(&display);
  return true;
}

void UIManager::nextView() {
  currentIndex = (currentIndex + 1) % viewCount;
  display.clearDisplay();
  views[currentIndex]->render(&display);
}

void UIManager::previousView() {
  currentIndex = (currentIndex - 1 + viewCount) % viewCount;
  display.clearDisplay();
  views[currentIndex]->render(&display);
}

void UIManager::applyInputHandlers() {
  // This method is no longer used since main.cpp handles input routing
}

// Method to handle clicks for the current view
void UIManager::handleClick() {
  auto clickHandler = views[currentIndex]->onClick();
  if (clickHandler) {
    clickHandler();
    // Re-render the view after handling the click
    display.clearDisplay();
    views[currentIndex]->render(&display);
  }
}

// Method to handle rotation with sub-view delegation
void UIManager::handleRotate(int direction) {
  // Check if current view has a sub-view active
  if (views[currentIndex]->hasSubView()) {
    // Delegate to the current view's rotation handler (which will handle sub-view logic)
    auto rotateHandler = views[currentIndex]->onRotate();
    if (rotateHandler) {
      rotateHandler(direction);
      // Re-render the view after handling the rotation
      display.clearDisplay();
      views[currentIndex]->render(&display);
    }
  } else {
    // No sub-view, handle global navigation
    if (direction > 0) {
      nextView();
    } else {
      previousView();
    }
  }
}

void UIManager::update() {
  views[currentIndex]->update(&display);
}
