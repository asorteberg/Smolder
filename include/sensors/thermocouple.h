#pragma once
#include <Arduino.h>
#include <max6675.h>

class ThermocoupleSensor {
private:
  int cs1Pin;      // Grill thermocouple CS
  int cs2Pin;      // Food thermocouple CS  
  int sckPin;      // Shared SPI clock
  int do1Pin;      // Grill thermocouple DO (Data Out)
  int do2Pin;      // Food thermocouple DO (Data Out)
  
  MAX6675* grillThermocouple;
  MAX6675* foodThermocouple;

public:
  ThermocoupleSensor(int cs1 = 13, int cs2 = 14, int sck = 18, int do1 = 19, int do2 = 23);
  void begin();
  float readGrillTemperature();
  float readFoodTemperature();
  bool hasError();
  void updateTemperatures();
};
