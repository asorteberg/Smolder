#pragma once

class UIView {
public:
  virtual void render() = 0;
  virtual void onClick() {}
  virtual void onRotate(int dir) {}
  virtual ~UIView() {}
};
