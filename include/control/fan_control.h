#pragma once
#include <Arduino.h>

class FanController {
public:
  void begin(int pwmPin);
  void setSpeed(uint8_t percent);  // 0–100
private:
  int _pin;
};