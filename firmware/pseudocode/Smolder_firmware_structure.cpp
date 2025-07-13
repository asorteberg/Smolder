// Smolder Firmware - Starter PlatformIO Structure

// main.cpp
#include <Arduino.h>
#include "scheduler.h"
#include "wifi.h"
#include "web_server.h"
#include "display_ui.h"
#include "encoder.h"
#include "thermocouple.h"
#include "fan_control.h"
#include "servo_control.h"
#include "eeprom.h"

void setup() {
  initLogger();
  initPreferences();
  initWiFi();
  initWebServer();
  initDisplay();
  initEncoder();
  initSensors();
  initActuators();

  scheduler.addTask("readTemps", 1000, readTemperatures);
  scheduler.addTask("updateUI", 500, drawUI);
  scheduler.addTask("fanControl", 500, updateFan);
}

void loop() {
  scheduler.run();
  checkRotaryInput();
  handleWiFiReconnect();
}

// scheduler.h (basic task scheduler using millis)
#ifndef SCHEDULER_H
#define SCHEDULER_H
#include <Arduino.h>
struct Task {
  String name;
  unsigned long interval;
  unsigned long lastRun;
  void (*callback)();
};

class Scheduler {
public:
  void addTask(String name, unsigned long interval, void (*callback)());
  void run();
private:
  Task tasks[10];
  int taskCount = 0;
};
extern Scheduler scheduler;
#endif

// wifi.h
void initWiFi();
void handleWiFiReconnect();

// web_server.h
void initWebServer();

// display_ui.h
void initDisplay();
void drawUI();

// encoder.h
void initEncoder();
void checkRotaryInput();

// thermocouple.h
void initSensors();
void readTemperatures();
double getGrillTemp();
double getFoodTemp();

// fan_control.h
void initActuators();
void updateFan();

// servo_control.h
void moveDamperTo(int angle);

// eeprom.h
void initPreferences();
void saveTargetTemp(int temp);
int loadTargetTemp();

// logger.cpp (utils/logger)
void initLogger() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("GrillBit booting...");
}
