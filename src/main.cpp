#include "ui/ui_manager.h"
#include "ui/views/temp_view.h"
#include "ui/views/settings_view.h"
#include "ui/ui_manager.h"
#include "ui/encoder.h"

#define PIN_ENCODER_CLK  2
#define PIN_ENCODER_DT   3
#define PIN_ENCODER_SW   4

// TempView tempView;
// SettingsView settingsView;

void setup() {
  encoderInput.begin(PIN_ENCODER_CLK, PIN_ENCODER_DT, PIN_ENCODER_SW);

  encoderInput.setOnRotate([](int dir) {
    uiManager.handleRotate(dir);
  });

  encoderInput.setOnClick([]() {
    uiManager.handleClick();
  });
  // Initialize display, rotary encoder, etc.

  //uiManager.addView(&tempView);
  //uiManager.addView(&settingsView);
  //uiManager.begin();
}

void loop() {
   encoderInput.update(); // polling loop
  // Handle encoder rotation & click
  // uiManager.update();
}
