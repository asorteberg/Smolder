#pragma once
#include <Adafruit_SSD1306.h>
#include <functional>

class UIView {
public:
  virtual void render(Adafruit_SSD1306* display) = 0;
  virtual void update(Adafruit_SSD1306* display) {}
  virtual std::function<void(int)> onRotate() = 0;
  virtual std::function<void()> onClick() = 0;
  
  // Sub-view management for hierarchical navigation
  virtual bool hasSubView() const { return subView != nullptr; }
  virtual UIView* getSubView() const { return subView; }
  virtual void setSubView(UIView* view) { subView = view; }
  virtual void clearSubView() { subView = nullptr; }
  
  // Navigation helper
  virtual bool isInSubView() const { return hasSubView(); }
  
  // Input handling priority:
  // 1. If view has sub-view, sub-view handles input
  // 2. If view has no sub-view, view handles input
  // 3. UIManager handles global navigation when no sub-view is active

protected:
  UIView* subView = nullptr;
};

