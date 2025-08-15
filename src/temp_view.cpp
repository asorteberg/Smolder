#include "ui/views/temp_view.h"
#include "ui/views/temp_change_view.h"

#include "config.h"


void TempView::render(Adafruit_SH1106G *display)
{
  // If we have a sub-view, render it instead
  if (hasSubView()) {
    getSubView()->render(display);
    return;
  }
  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SH110X_WHITE);
  const char *text = "SMOLDER UI";
  int text_width = strlen(text) * 6; // 6 pixels per character
  int text_x = (128 - text_width) / 2;
  display->setCursor(text_x, 0);
  display->println(text);
  display->println("=====================");
  display->print("  Actual     Target");
  display->setCursor(10, 25);
  display->setTextSize(2);
  display->print(grillTemp);
  display->setTextSize(1);
  display->print("F");
  display->setTextSize(2);
  display->setCursor(76, 25);
  display->print(setPoint);
  display->setTextSize(1);
  display->print("F");
  display->setCursor(18, 44);
  display->print("Probe Temp: ");
  display->print(foodTemp);
  display->print("F");
  if (editing)
  {
    display->print(" <- EDIT");
  }
  display->setCursor(24, 56);
  display->print("Click to ");
  display->print(editing ? "save" : "edit");
  display->display();
}
std::function<void(int)> TempView::onRotate()
{
  return [this](int dir)
  { handleRotate(dir); };
}

std::function<void()> TempView::onClick()
{
  return [this]()
  { handleClick(); };
}
void TempView::handleClick()
{
  // If we have a sub-view, delegate to it
  if (hasSubView()) {
    auto subViewClick = getSubView()->onClick();
    if (subViewClick) {
      subViewClick();
    }
  } else {
    // Enter temp change view
    enterTempChangeView();
  }
}

void TempView::handleRotate(int dir)
{
  if (hasSubView()) {
    auto subViewRotate = getSubView()->onRotate();
    if (subViewRotate) {
      subViewRotate(dir);
    }
    return;
  }
}

void TempView::enterTempChangeView()
{
  if (!tempChangeView)
  {
    tempChangeView = new TempChangeView();

    // Set up callback for returning to main settings view
    tempChangeView->setOnBackCallback([this](){
       exitTempChangeView(); 
    });
  }

  setSubView(tempChangeView);
}

void TempView::exitTempChangeView()
{
  Serial.print("Exiting TempChangeView with setPoint: ");
  clearSubView();
}
