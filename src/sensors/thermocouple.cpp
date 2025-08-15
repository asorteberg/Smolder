#include "sensors/thermocouple.h"
#include "config.h"
#include <SPI.h>

extern int foodTemp;
extern int grillTemp;

ThermocoupleSensor::ThermocoupleSensor(int cs1, int cs2, int sck, int miso) 
  : cs1Pin(cs1), cs2Pin(cs2), sckPin(sck), misoPin(miso) {
}

void ThermocoupleSensor::begin() {
  pinMode(cs1Pin, OUTPUT);
  pinMode(cs2Pin, OUTPUT);
  digitalWrite(cs1Pin, HIGH);
  digitalWrite(cs2Pin, HIGH);
  
  SPI.begin(sckPin, misoPin, -1, -1);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.setDataMode(SPI_MODE0);
}

float ThermocoupleSensor::readGrillTemperature() {
  uint16_t value = 0;
  
  digitalWrite(cs1Pin, LOW);
  delayMicroseconds(1);
  
  value = SPI.transfer(0x00) << 8;
  value |= SPI.transfer(0x00);
  
  digitalWrite(cs1Pin, HIGH);
  
  if (value & 0x4) {
    return NAN;
  }
  
  value >>= 3;
  return value * 0.25;
}

float ThermocoupleSensor::readFoodTemperature() {
  uint16_t value = 0;
  
  digitalWrite(cs2Pin, LOW);
  delayMicroseconds(1);
  
  value = SPI.transfer(0x00) << 8;
  value |= SPI.transfer(0x00);
  
  digitalWrite(cs2Pin, HIGH);
  
  if (value & 0x4) {
    return NAN;
  }
  
  value >>= 3;
  return value * 0.25;
}

bool ThermocoupleSensor::hasError() {
  float grillTemp = readGrillTemperature();
  float foodTemp = readFoodTemperature();
  return isnan(grillTemp) || isnan(foodTemp);
}

void ThermocoupleSensor::updateTemperatures() {
  float grillTempF = readGrillTemperature();
  float foodTempF = readFoodTemperature();
  
  if (!isnan(grillTempF)) {
    ::grillTemp = (int)grillTempF;
  }
  
  if (!isnan(foodTempF)) {
    ::foodTemp = (int)foodTempF;
  }
}
