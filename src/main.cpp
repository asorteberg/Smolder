#include "input_manager.h"
#include "ui/ui_manager.h"

Adafruit_SSD1306 display;
InputManager inputManager;
UIManager uiManager(inputManager);

// Handle rotation with sub-view delegation
void handleGlobalRotate(int direction) {
  Serial.println(direction);
  uiManager.handleRotate(direction);
}

// Selection with center click
void handleGlobalClick() {
  Serial.println("Center click - view specific action");
  uiManager.handleClick();
}

void setup() {

  Serial.begin(115200);
  Serial.println("Smolder Controller Starting...");
  
  inputManager.begin(2, 23, 4); // CLK, DT, SW
  
  // Set up global input handlers
  inputManager.setRotateHandler(handleGlobalRotate);
  inputManager.setClickHandler(handleGlobalClick);
  
  if (!uiManager.begin()) {
    Serial.println("Failed to initialize display!");
    while (1);
  }
  
  Serial.println("System initialized successfully");
}

void loop() {
  inputManager.update();
  uiManager.update();
  delay(10);
}
