#include "pid.h"

void PIDController::begin(float kp, float ki, float kd) {
  _kp = kp;
  _ki = ki;
  _kd = kd;
  _prevError = 0;
  _integral = 0;
}

float PIDController::compute(float setpoint, float input) {
  float error = setpoint - input;
  _integral += error;
  float derivative = error - _prevError;
  _prevError = error;
  return _kp * error + _ki * _integral + _kd * derivative;
}
