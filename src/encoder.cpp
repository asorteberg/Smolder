#include "ui/encoder.h"
#include <Arduino.h>

EncoderInput encoderInput;

void EncoderInput::begin(int clkPin, int dtPin, int swPin) {
  _clkPin = clkPin;
  _dtPin = dtPin;
  _swPin = swPin;
  pinMode(_clkPin, INPUT_PULLUP);
  pinMode(_dtPin, INPUT_PULLUP);
  pinMode(_swPin, INPUT_PULLUP);
  _lastState = digitalRead(_clkPin);
}

void EncoderInput::setOnRotate(std::function<void(int)> callback) {
  _rotateCallback = callback;
}

void EncoderInput::setOnClick(std::function<void()> callback) {
  _clickCallback = callback;
}

void EncoderInput::update() {
  int clkState = digitalRead(_clkPin);
  int dtState = digitalRead(_dtPin);

  // Trigger on falling edge of CLK only
  if (_lastState == HIGH && clkState == LOW) {
    if (dtState == HIGH) {
      if (_rotateCallback) _rotateCallback(1); // CW
    } else {
      if (_rotateCallback) _rotateCallback(-1); // CCW
    }
  }

  _lastState = clkState;

  // Button click detection
  static bool lastButtonState = HIGH;
  bool currentButtonState = digitalRead(_swPin);
  if (lastButtonState == HIGH && currentButtonState == LOW) {
    if (_clickCallback) _clickCallback();
  }
  lastButtonState = currentButtonState;

}

