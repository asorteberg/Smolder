#include "sensors/thermocouple.h"
#include "config.h"

extern int foodTemp;
extern int grillTemp;

ThermocoupleSensor::ThermocoupleSensor(int cs1, int cs2, int sck, int do1, int do2) 
  : cs1Pin(cs1), cs2Pin(cs2), sckPin(sck), do1Pin(do1), do2Pin(do2) {
  grillThermocouple = nullptr;
  foodThermocouple = nullptr;
}

void ThermocoupleSensor::begin() {
  Serial.println("=== ThermocoupleSensor::begin() with MAX6675 Library ===");
  Serial.print("CS1 Pin (Grill): "); Serial.println(cs1Pin);
  Serial.print("CS2 Pin (Food): "); Serial.println(cs2Pin);
  Serial.print("SCK Pin (shared): "); Serial.println(sckPin);
  Serial.print("DO1 Pin (Grill): "); Serial.println(do1Pin);
  Serial.print("DO2 Pin (Food): "); Serial.println(do2Pin);
  
  // Initialize MAX6675 objects - each with dedicated DO pin
  grillThermocouple = new MAX6675(sckPin, cs1Pin, do1Pin);
  foodThermocouple = new MAX6675(sckPin, cs2Pin, do2Pin);
  
  Serial.println("MAX6675 library initialized for both thermocouples");
  
  // Wait a bit for sensors to stabilize
  delay(500);
}

float ThermocoupleSensor::readGrillTemperature() {
  if (!grillThermocouple) {
    Serial.println("GRILL - ERROR: MAX6675 not initialized!");
    return NAN;
  }
  
  Serial.print("GRILL - Reading with MAX6675 library... ");
  
  float tempC = grillThermocouple->readCelsius();
  float tempF = grillThermocouple->readFahrenheit();
  
  Serial.print("Raw: "); Serial.print(tempC, 2); Serial.print("C / "); Serial.print(tempF, 2); Serial.print("F");
  
  // Check for sensor errors (NAN indicates sensor issues)
  if (isnan(tempC) || isnan(tempF)) {
    Serial.println(" - ERROR: Sensor not responding or disconnected!");
    return NAN;
  }
  
  // Sanity check for reasonable temperatures
  if (tempC < -40 || tempC > 1000) {
    Serial.print(" - WARNING: Temperature out of range! ("); Serial.print(tempC); Serial.println("C)");
  } else {
    Serial.println(" - OK");
  }
  
  return tempF;
}

float ThermocoupleSensor::readFoodTemperature() {
  if (!foodThermocouple) {
    Serial.println("FOOD - ERROR: MAX6675 not initialized!");
    return NAN;
  }
  
  Serial.print("FOOD - Reading with MAX6675 library... ");
  
  float tempC = foodThermocouple->readCelsius();
  float tempF = foodThermocouple->readFahrenheit();
  
  Serial.print("Raw: "); Serial.print(tempC, 2); Serial.print("C / "); Serial.print(tempF, 2); Serial.print("F");
  
  // Check for sensor errors (NAN indicates sensor issues)
  if (isnan(tempC) || isnan(tempF)) {
    Serial.println(" - ERROR: Sensor not responding or disconnected!");
    return NAN;
  }
  
  // Sanity check for reasonable temperatures
  if (tempC < -40 || tempC > 1000) {
    Serial.print(" - WARNING: Temperature out of range! ("); Serial.print(tempC); Serial.println("C)");
  } else {
    Serial.println(" - OK");
  }
  
  return tempF;
}

bool ThermocoupleSensor::hasError() {
  float grillTemp = readGrillTemperature();
  float foodTemp = readFoodTemperature();
  return isnan(grillTemp) || isnan(foodTemp);
}

void ThermocoupleSensor::updateTemperatures() {
  Serial.println("\n=== Reading Thermocouples with MAX6675 Library ===");
  float grillTempF = readGrillTemperature();
  float foodTempF = readFoodTemperature();
  
  Serial.print("Results - Grill: ");
  if (!isnan(grillTempF)) {
    ::grillTemp = (int)grillTempF;
    Serial.print(grillTempF, 1); Serial.print("F -> "); Serial.print(::grillTemp); Serial.println("F (int)");
  } else {
    Serial.println("NAN (keeping previous value)");
  }
  
  Serial.print("Results - Food: ");
  if (!isnan(foodTempF)) {
    ::foodTemp = (int)foodTempF;
    Serial.print(foodTempF, 1); Serial.print("F -> "); Serial.print(::foodTemp); Serial.println("F (int)");
  } else {
    Serial.println("NAN (keeping previous value)");
  }
  
  Serial.print("Global variables now: grillTemp="); Serial.print(::grillTemp);
  Serial.print("F, foodTemp="); Serial.print(::foodTemp); Serial.println("F");
  Serial.println("==================================================\n");
}