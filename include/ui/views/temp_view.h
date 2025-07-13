#pragma once
#include "../view_base.h"
#include <Arduino.h>

class TempView : public UIView {
public:
  void render() override;
  void onClick() override;
  void onRotate(int dir) override;

private:
  bool editing = false;
  int setPoint = 225; // Default temp
};
