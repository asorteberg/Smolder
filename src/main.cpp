#include "input_manager.h"
#include "ui/ui_manager.h"
#include "sensors/thermocouple.h"

InputManager inputManager;
UIManager uiManager(inputManager);
ThermocoupleSensor thermocouple; // Uses default pins: cs1=13, cs2=14, sck=18, miso=19

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
  
  inputManager.begin(2, 15, 4); // CLK, DT, SW
  
  // Set up global input handlers
  inputManager.setRotateHandler(handleGlobalRotate);
  inputManager.setClickHandler(handleGlobalClick);
  
  // Initialize thermocouple sensor
  thermocouple.begin();
  Serial.println("Thermocouple sensor initialized");
  
  if (!uiManager.begin()) {
    Serial.println("Failed to initialize display!");
    while (1);
  }
  
  Serial.println("System initialized successfully");
}

void loop() {
  inputManager.update();
  uiManager.update();
  
  // Update thermocouple readings (updates global grillTemp and foodTemp)
  thermocouple.updateTemperatures();
  
  delay(10);
}
