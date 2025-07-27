#include "ui/views/temp_change_view.h"
#include "config.h"


TempChangeView::TempChangeView() {}

void TempChangeView::render(Adafruit_SSD1306* display) {
    display->clearDisplay();
    display->setTextSize(2);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(10, 25);
    display->print(setPoint);
    display->setTextSize(1);
    display->print("F");
    display->setCursor(24, 56);
    display->print("Click to save");
    display->display();
}

std::function<void(int)> TempChangeView::onRotate() {
    return [this](int dir) { handleRotate(dir); };
}

std::function<void()> TempChangeView::onClick() {
    return [this]() { handleClick(); };
}

void TempChangeView::handleRotate(int dir) {
    setPoint += (dir > 0) ? 5 : -5;
    if (setPoint < 100) setPoint = 100;
    if (setPoint > 500) setPoint = 500;
    
}

void TempChangeView::handleClick() {

    Serial.print("Clicked: ");
    
    onBackCallback();
    
    // Optionally: trigger a callback to parent view to save and exit
}