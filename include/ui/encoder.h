#pragma once
#include <Arduino.h>

class EncoderInput {
public:
  void begin(uint8_t clkPin, uint8_t dtPin, uint8_t swPin);
  void update();  // Call in loop() if polling

  void setOnRotate(void (*callback)(int));
  void setOnClick(void (*callback)());

private:
  uint8_t _clkPin, _dtPin, _swPin;
  int _lastClk = HIGH;
  bool _lastButton = HIGH;
  unsigned long _lastDebounce = 0;

  void (*rotateCallback)(int) = nullptr;
  void (*clickCallback)() = nullptr;
};

extern EncoderInput encoderInput;
