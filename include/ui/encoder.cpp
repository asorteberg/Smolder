#include "encoder.h"

EncoderInput encoderInput;

void EncoderInput::begin(uint8_t clkPin, uint8_t dtPin, uint8_t swPin) {
  _clkPin = clkPin;
  _dtPin = dtPin;
  _swPin = swPin;

  pinMode(_clkPin, INPUT_PULLUP);
  pinMode(_dtPin, INPUT_PULLUP);
  pinMode(_swPin, INPUT_PULLUP);
}

void EncoderInput::setOnRotate(void (*callback)(int)) {
  rotateCallback = callback;
}

void EncoderInput::setOnClick(void (*callback)()) {
  clickCallback = callback;
}

void EncoderInput::update() {
  int clk = digitalRead(_clkPin);
  int dt = digitalRead(_dtPin);

  // Check rotation
  if (clk != _lastClk && clk == LOW) {
    int direction = (dt == HIGH) ? 1 : -1;
    if (rotateCallback) rotateCallback(direction);
  }
  _lastClk = clk;

  // Check button (with debounce)
  bool reading = digitalRead(_swPin);
  if (reading != _lastButton) {
    _lastDebounce = millis();
  }

  if ((millis() - _lastDebounce) > 50) {
    if (_lastButton == HIGH && reading == LOW) {
      if (clickCallback) clickCallback();
    }
  }

  _lastButton = reading;
}
