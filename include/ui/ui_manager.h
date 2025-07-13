#pragma once
#include "view_base.h"

#define MAX_VIEWS 5

class UIManager {
public:
  void begin();
  void update();
  void nextView();
  void prevView();
  void handleClick();
  void handleRotate(int dir);

private:
  UIView* views[MAX_VIEWS];
  int viewCount = 0;
  int currentIndex = 0;
public:
  void addView(UIView* view);
};

extern UIManager uiManager;
