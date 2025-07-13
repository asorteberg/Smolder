#include "ui/ui_manager.h"

UIManager uiManager;

void UIManager::begin() {
  currentIndex = 0;
}

void UIManager::addView(UIView* view) {
  if (viewCount < MAX_VIEWS) {
    views[viewCount++] = view;
  }
}

void UIManager::update() {
  views[currentIndex]->render();
}

void UIManager::nextView() {
  currentIndex = (currentIndex + 1) % viewCount;
}

void UIManager::prevView() {
  currentIndex = (currentIndex - 1 + viewCount) % viewCount;
}

void UIManager::handleClick() {
  views[currentIndex]->onClick();
}

void UIManager::handleRotate(int dir) {
  views[currentIndex]->onRotate(dir);
}
