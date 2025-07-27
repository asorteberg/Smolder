#pragma once
#include <ESP32Servo.h>

class ServoController {
public:
  void begin(int pin = 5);
  void setAngle(int angle);
  void setPosition(float percentage);
  int getCurrentAngle();
  bool isAttached();
  void detach();
  
private:
  Servo _servo;
  int _currentAngle = 90;
  int _pin = 5;
};
