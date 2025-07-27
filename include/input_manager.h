#pragma once
#include <functional>
#include <Arduino.h>

class InputManager {
public:
  void begin(uint8_t clkPin, uint8_t dtPin, uint8_t swPin);
  void update();

  void setRotateHandler(std::function<void(int)> handler);
  void setClickHandler(std::function<void()> handler);

private:
  uint8_t _clkPin, _dtPin, _swPin;
  int _lastClkState = HIGH;
  int _lastDtState = HIGH;
  int _lastBtnState = HIGH;
  std::function<void(int)> rotateHandler;
  std::function<void()> clickHandler;
};