#pragma once
#include "../ui_view.h"

class TempChangeView;
class TempView : public UIView
{
public:
  void render(Adafruit_SSD1306 *display) override;
  std::function<void(int)> onRotate() override;
  std::function<void()> onClick() override;

private:
  TempChangeView *tempChangeView = nullptr;
  int setPoint = 225;
  bool editing = false;

  void handleRotate(int dir);
  void handleClick();

  void enterTempChangeView();
  void exitTempChangeView();
};