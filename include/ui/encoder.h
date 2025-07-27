#pragma once
#include <functional>

class EncoderInput {
public:
  void begin(int clkPin, int dtPin, int swPin);
  void update();

  void setOnRotate(std::function<void(int)> callback);
  void setOnClick(std::function<void()> callback);

private:
  int _clkPin, _dtPin, _swPin;
  int _lastState = 0;
  std::function<void(int)> _rotateCallback;
  std::function<void()> _clickCallback;
};

extern EncoderInput encoderInput;
