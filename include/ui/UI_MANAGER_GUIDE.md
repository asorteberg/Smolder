# UI Manager System Documentation

## Overview

The UI Manager system provides a flexible, hierarchical view management system for the Smolder ESP32 Kamado grill controller. It supports multiple views with sub-view navigation, input delegation, and consistent rendering on the SH1106 OLED display.

## Architecture

### Core Components

- **UIManager** (`include/ui/ui_manager.h`): Main controller managing views and input delegation
- **UIView** (`include/ui/ui_view.h`): Abstract base class for all views
- **InputManager** (`include/input_manager.h`): Handles KY-040 rotary encoder input
- **Views** (`include/ui/views/`): Concrete view implementations

### File Structure

```
include/ui/
├── ui_manager.h           # Main UI controller
├── ui_view.h              # Base view class
├── views/
│   ├── temp_view.h        # Temperature monitoring view
│   ├── settings_view.h    # Settings main view
│   └── settings_list_view.h # Settings menu sub-view
└── UI_MANAGER_GUIDE.md    # This documentation

src/
├── ui_manager.cpp         # UIManager implementation
├── temp_view.cpp          # Temperature view implementation
├── settings_view.cpp      # Settings view implementation
└── settings_list_view.cpp # Settings list implementation
```

## Current View System

### Active Views

1. **TempView** (`temp_view.h/.cpp`)
   - Displays grill and food temperatures
   - Shows target temperature setpoint
   - Allows editing target temperature with encoder
   - No sub-views

2. **SettingsView** (`settings_view.h/.cpp`)
   - Main settings screen with gear icon
   - Launches SettingsListView sub-view on click
   - Manages sub-view lifecycle

3. **SettingsListView** (`settings_list_view.h/.cpp`)
   - Scrollable menu with 8 settings options
   - Cursor navigation with encoder rotation
   - "< Back" option to return to parent view

### View Navigation Flow

```
TempView ←→ SettingsView
              ↓ (click)
         SettingsListView
              ↑ (back)
```

## Input Handling System

### Hierarchical Input Delegation

The system uses a three-tier input handling approach:

1. **UIManager Level**: Global input coordination
2. **View Level**: View-specific input handling
3. **Sub-View Level**: Sub-view input handling when active

### Input Flow

```
main.cpp handlers
    ↓
UIManager.handleRotate() / handleClick()
    ↓
Current View's onRotate() / onClick()
    ↓ (if sub-view active)
Sub-View's onRotate() / onClick()
```

### Encoder Behavior

- **No Sub-View Active**: Rotation switches between main views (TempView ↔ SettingsView)
- **Sub-View Active**: Rotation delegated to sub-view (e.g., menu navigation)
- **Editing Mode**: View handles rotation internally (e.g., temperature adjustment)

## Creating New Views

### 1. Create Header File

```cpp
// include/ui/views/my_view.h
#pragma once
#include "../ui_view.h"

class MyView : public UIView {
public:
    MyView();
    void render(Adafruit_SSD1306* display) override;
    std::function<void(int)> onRotate() override;
    std::function<void()> onClick() override;

private:
    void handleRotate(int dir);
    void handleClick();
    
    // Your view-specific data
    int myData = 0;
};
```

### 2. Create Implementation File

```cpp
// src/my_view.cpp
#include "ui/views/my_view.h"

MyView::MyView() {
    // Initialize your view
}

void MyView::render(Adafruit_SSD1306* display) {
    // Check for sub-view first
    if (hasSubView()) {
        getSubView()->render(display);
        return;
    }
    
    // Render your view
    display->clearDisplay();
    display->setTextSize(1);
    display->setTextColor(SSD1306_WHITE);
    display->setCursor(0, 0);
    display->println("My View");
    display->display();
}

std::function<void(int)> MyView::onRotate() {
    return [this](int dir) { handleRotate(dir); };
}

std::function<void()> MyView::onClick() {
    return [this]() { handleClick(); };
}

void MyView::handleRotate(int dir) {
    // If sub-view is active, delegate to it
    if (hasSubView()) {
        auto subViewRotate = getSubView()->onRotate();
        if (subViewRotate) {
            subViewRotate(dir);
        }
        return;
    }
    
    // Handle rotation for this view
    myData += (dir > 0) ? 1 : -1;
}

void MyView::handleClick() {
    // If sub-view is active, delegate to it
    if (hasSubView()) {
        auto subViewClick = getSubView()->onClick();
        if (subViewClick) {
            subViewClick();
        }
        return;
    }
    
    // Handle click for this view
    // Example: Toggle between modes, enter sub-view, etc.
}
```

### 3. Register View in UIManager

```cpp
// In ui_manager.cpp, modify begin() method
bool UIManager::begin() {
    // ... existing code ...
    
    views[0] = new TempView();
    views[1] = new SettingsView();
    views[2] = new MyView();  // Add your view
    viewCount = 3;           // Update count
    
    // ... rest of method ...
}
```

## Sub-View System

### Creating Sub-Views

Sub-views are regular UIView objects managed by parent views:

```cpp
// In parent view
void MyView::enterSubView() {
    if (!mySubView) {
        mySubView = new MySubView();
        
        // Set up callbacks if needed
        mySubView->setOnExitCallback([this]() {
            exitSubView();
        });
    }
    
    setSubView(mySubView);  // Activate sub-view
}

void MyView::exitSubView() {
    clearSubView();  // Deactivate sub-view
}
```

### Sub-View Best Practices

1. **Lifecycle Management**: Parent view owns sub-view objects
2. **Callback Pattern**: Use callbacks for sub-view to parent communication
3. **Input Delegation**: Always check for sub-view in input handlers
4. **Render Delegation**: Check for sub-view in render method

## Click Handling Within Views

### Basic Click Handling

```cpp
void MyView::handleClick() {
    // Delegate to sub-view if active
    if (hasSubView()) {
        auto subViewClick = getSubView()->onClick();
        if (subViewClick) {
            subViewClick();
        }
        return;
    }
    
    // Handle click based on view state
    switch (currentState) {
        case STATE_NORMAL:
            // Enter edit mode, show menu, etc.
            break;
        case STATE_EDITING:
            // Save changes, exit edit mode
            break;
    }
}
```

### Multi-State Click Handling

```cpp
// Example: Temperature view with editing
void TempView::handleClick() {
    if (editing) {
        // Save temperature setting
        editing = false;
        // Could save to EEPROM, send to controller, etc.
    } else {
        // Enter edit mode
        editing = true;
    }
}
```

## Display Management

### Rendering Guidelines

1. **Clear Display**: Always call `display->clearDisplay()` first
2. **Set Text Properties**: Configure size, color, cursor position
3. **Display Content**: Use print/println for text, drawBitmap for graphics
4. **Update Display**: Call `display->display()` to show changes

### Display Specifications

- **Resolution**: 128x64 pixels
- **Controller**: SH1106 (I2C)
- **Text**: 6 pixels wide per character (size 1)
- **Visible Lines**: ~8 lines at size 1 (8 pixels per line)

### Common Display Patterns

```cpp
// Text display
display->clearDisplay();
display->setTextSize(1);
display->setTextColor(SSD1306_WHITE);
display->setCursor(0, 0);
display->println("Title");
display->drawLine(0, 10, 128, 10, SSD1306_WHITE);  // Underline

// Centered text
const char* text = "Centered";
int textWidth = strlen(text) * 6;
int x = (128 - textWidth) / 2;
display->setCursor(x, 20);
display->print(text);

// Bitmap display (32x32 example)
int x = (128 - 32) / 2;  // Center horizontally
int y = 16;
display->drawBitmap(x, y, my_bitmap, 32, 32, SSD1306_WHITE);

display->display();
```

## Input Manager Integration

### Encoder Setup

```cpp
// In main.cpp
InputManager inputManager;
UIManager uiManager(inputManager);

void setup() {
    inputManager.begin(2, 19, 4);  // CLK, DT, SW pins
    inputManager.setRotateHandler(handleGlobalRotate);
    inputManager.setClickHandler(handleGlobalClick);
}

void handleGlobalRotate(int direction) {
    uiManager.handleRotate(direction);
}

void handleGlobalClick() {
    uiManager.handleClick();
}
```

### Encoder Directions

- **Positive Direction**: Clockwise rotation (typically "next", "up", "increase")
- **Negative Direction**: Counter-clockwise rotation (typically "previous", "down", "decrease")

## Best Practices

### View Design

1. **Keep Views Focused**: Each view should have a single responsibility
2. **Consistent Navigation**: Use similar patterns across views
3. **Clear Visual Hierarchy**: Make it obvious what's selected/active
4. **Responsive Input**: Provide immediate visual feedback

### Memory Management

1. **Stack Allocation**: Use stack allocation for simple views when possible
2. **Heap Cleanup**: Delete sub-views in parent destructors
3. **Avoid Memory Leaks**: Always pair new/delete calls

### Code Organization

1. **Separate Concerns**: Keep rendering, input handling, and business logic separate
2. **Use Callbacks**: Prefer callbacks over tight coupling between views
3. **Document State**: Clearly document view states and transitions

### Performance

1. **Minimize Redraws**: Only redraw when necessary
2. **Efficient Rendering**: Use appropriate text sizes and minimal graphics
3. **Non-Blocking**: Keep all operations non-blocking for cooperative multitasking

## Future Enhancements

### Planned Features

- **View Transitions**: Smooth transitions between views
- **Modal Dialogs**: Temporary overlay views
- **Themes**: Support for different display themes
- **View Stack**: Navigation history for deep hierarchies

### Extension Points

- **Custom Input Types**: Support for additional input methods
- **View Persistence**: Save/restore view states
- **Dynamic Views**: Runtime view creation and destruction
- **Event System**: Pub/sub pattern for view communication

## Troubleshooting

### Common Issues

1. **View Not Switching**: Check that view is registered in UIManager
2. **Input Not Working**: Verify input delegation in parent view
3. **Display Not Updating**: Ensure `display->display()` is called
4. **Memory Issues**: Check for proper cleanup in destructors

### Debug Tips

1. **Serial Output**: Add Serial.println() for debugging state changes
2. **Display Debugging**: Show debug info on screen temporarily
3. **Input Logging**: Log encoder input values and directions
4. **View State**: Display current view and sub-view status

## Examples

See existing views for complete examples:
- **TempView**: Simple view with internal state
- **SettingsView**: Parent view with sub-view management
- **SettingsListView**: Sub-view with scrollable menu

---

*This documentation should be updated as new views and features are added to the system.*