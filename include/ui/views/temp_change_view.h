#pragma once
#include "../ui_view.h"

class TempChangeView : public UIView {
public:
    TempChangeView();
    void render(Adafruit_SSD1306* display) override;
    std::function<void(int)> onRotate() override;
    std::function<void()> onClick() override;
    bool editing = true;
    //int getSetPoint() const { return setPoint; }

    void setOnBackCallback(std::function<void()> callback) { onBackCallback = callback; }

private:
    void handleRotate(int dir);
    void handleClick();
    //bool editing = true;
    std::function<void()> onBackCallback;
};