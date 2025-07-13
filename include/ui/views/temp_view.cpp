#include "temp_view.h"
#include <Adafruit_SSD1306.h>  // Or U8g2, depending on what you're using

extern Adafruit_SSD1306 display;
extern double currentTemp;

void TempView::render() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Grill Temp: ");
  display.print(currentTemp);
  display.println(" F");

  display.setCursor(0, 16);
  display.print("Set Point: ");
  display.print(setPoint);
  if (editing) display.print(" *");

  display.display();
}

void TempView::onClick() {
  editing = !editing;
}

void TempView::onRotate(int dir) {
  if (editing) {
    setPoint += dir * 5;
    if (setPoint < 100) setPoint = 100;
    if (setPoint > 450) setPoint = 450;
  } else {
    if (dir > 0) uiManager.nextView();
    else uiManager.prevView();
  }
}
