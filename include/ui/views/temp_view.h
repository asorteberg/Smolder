#pragma once
#include "../ui_view.h"

class TempChangeView;
class TempView : public UIView
{
public:
  void render(Adafruit_SH1106G *display) override;
  std::function<void(int)> onRotate() override;
  std::function<void()> onClick() override;

private:
  TempChangeView *tempChangeView = nullptr;
  bool editing = false;

  void handleRotate(int dir);
  void handleClick();

  void enterTempChangeView();
  void exitTempChangeView();
};