#include "input_manager.h"
#include "ui/ui_manager.h"
#include "sensors/thermocouple.h"
#include "config.h"

InputManager inputManager;
UIManager uiManager(inputManager);
ThermocoupleSensor thermocouple; // Uses default pins: cs1=13, cs2=14, sck=18, do1=19, do2=23

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
  
  // Show initial config values
  Serial.print("Initial config values - grillTemp: "); Serial.print(grillTemp);
  Serial.print("F, foodTemp: "); Serial.print(foodTemp);
  Serial.print("F, setPoint: "); Serial.print(setPoint); Serial.println("F");
  
  if (!uiManager.begin()) {
    Serial.println("Failed to initialize display!");
    while (1);
  }
  
  Serial.println("System initialized successfully");
}

void loop() {
  static unsigned long lastTempUpdate = 0;
  const unsigned long TEMP_UPDATE_INTERVAL = 2000; // Read thermocouples every 2 seconds
  
  inputManager.update();
  uiManager.update();
  
  // Update thermocouple readings at a slower rate to avoid spam
  unsigned long currentTime = millis();
  if (currentTime - lastTempUpdate >= TEMP_UPDATE_INTERVAL) {
    Serial.print("Loop count: "); Serial.print(currentTime/10); 
    Serial.println(" - Reading thermocouples...");
    thermocouple.updateTemperatures();
    lastTempUpdate = currentTime;
  }
  
  delay(10);
}
