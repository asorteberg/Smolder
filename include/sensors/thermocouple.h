#pragma once
#include <Arduino.h>

class ThermocoupleSensor {
private:
  int cs1Pin;
  int cs2Pin;
  int sckPin;
  int misoPin;

public:
  ThermocoupleSensor(int cs1 = 13, int cs2 = 14, int sck = 18, int miso = 19);
  void begin();
  float readGrillTemperature();
  float readFoodTemperature();
  bool hasError();
  void updateTemperatures();
};
