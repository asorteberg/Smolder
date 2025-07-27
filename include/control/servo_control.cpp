#include "servo_control.h"

void ServoController::begin(int pin) {
  _pin = pin;
  _servo.attach(_pin);
  _servo.write(_currentAngle);
}

void ServoController::setAngle(int angle) {
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;
  
  _currentAngle = angle;
  _servo.write(angle);
}

void ServoController::setPosition(float percentage) {
  if (percentage < 0.0) percentage = 0.0;
  if (percentage > 100.0) percentage = 100.0;
  
  int angle = (int)(percentage * 1.8);
  setAngle(angle);
}

int ServoController::getCurrentAngle() {
  return _currentAngle;
}

bool ServoController::isAttached() {
  return _servo.attached();
}

void ServoController::detach() {
  _servo.detach();
}