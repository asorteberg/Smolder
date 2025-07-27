#include "fan_control.h"

void FanController::begin(int pwmPin) {
  _pin = pwmPin;
  pinMode(_pin, OUTPUT);
}

void FanController::setSpeed(uint8_t percent) {
  analogWrite(_pin, map(percent, 0, 100, 0, 255));
}