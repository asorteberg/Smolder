#include "settings_view.h"
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;

void SettingsView::render() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Settings");

  for (int i = 0; i < options.size(); i++) {
    display.setCursor(0, 12 + i * 10);
    if (i == selected) display.print("> ");
    else display.print("  ");
    display.println(options[i]);
  }

  display.display();
}

void SettingsView::onClick() {
  // For now just print the option
  Serial.print("Selected: ");
  Serial.println(options[selected]);
}

void SettingsView::onRotate(int dir) {
  selected += dir;
  if (selected < 0) selected = 0;
  if (selected >= options.size()) selected = options.size() - 1;
}
