#pragma once

class PIDController {
public:
  void begin(float kp, float ki, float kd);
  float compute(float setpoint, float input);
private:
  float _kp, _ki, _kd;
  float _prevError, _integral;
};