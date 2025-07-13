#pragma once
#include "../view_base.h"
#include <Arduino.h>
#include <vector>
#include <string>

class SettingsView : public UIView {
public:
  void render() override;
  void onClick() override;
  void onRotate(int dir) override;

private:
  int selected = 0;
  std::vector<String> options = {
    "Wi-Fi Setup",
    "Fan Test",
    "PID Tuning",
    "Factory Reset"
  };
};
