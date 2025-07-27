#include "input_manager.h"

void InputManager::begin(uint8_t clkPin, uint8_t dtPin, uint8_t swPin) {
  _clkPin = clkPin;
  _dtPin = dtPin;
  _swPin = swPin;
  
  pinMode(_clkPin, INPUT_PULLUP);
  pinMode(_dtPin, INPUT_PULLUP);
  pinMode(_swPin, INPUT_PULLUP);
  
  _lastClkState = digitalRead(_clkPin);
  _lastDtState = digitalRead(_dtPin);
  _lastBtnState = digitalRead(_swPin);
}


unsigned long lastRotateTime = 0;
const unsigned long debounceDelay = 5;

void InputManager::update() {
  // Handle rotation
  int clkState = digitalRead(_clkPin);
  int dtState = digitalRead(_dtPin);


  // Detect rotation on CLK falling edge
  if (clkState != _lastClkState && clkState == LOW) {
    int direction = (dtState != clkState) ? 1 : -1;
    if (rotateHandler) {
      rotateHandler(direction);
    }
  }
  
  _lastClkState = clkState;
  _lastDtState = dtState;
  
  // Handle button click
  int btnState = digitalRead(_swPin);
  if (btnState != _lastBtnState && btnState == LOW) {
    if (clickHandler) {
      clickHandler();
    }
  }
  _lastBtnState = btnState;
  //delay(200);
}

void InputManager::setRotateHandler(std::function<void(int)> handler) {
  rotateHandler = handler;
}

void InputManager::setClickHandler(std::function<void()> handler) {
  clickHandler = handler;
}