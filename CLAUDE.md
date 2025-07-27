# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build and Development Commands

This is a PlatformIO-based ESP32 project:

```bash
# Build the firmware
pio run

# Upload to ESP32 device
pio run --target upload

# Monitor serial output
pio device monitor

# Clean build files
pio run --target clean

# Test with Wokwi simulator (requires wokwi-cli)
wokwi
```

## Architecture Overview

**Smolder** is an ESP32-based Kamado grill controller with a modular, cooperative multitasking architecture designed for cost-effectiveness and full customizability.

### Core Design Patterns

- **Manager-Component Pattern**: Each subsystem has a manager class (InputManager, UIManager) that coordinates components
- **Event-Driven Input**: Uses `std::function` callbacks for rotary encoder handling rather than polling
- **View State Machine**: UIManager maintains an array of polymorphic UIView objects for screen navigation
- **Cooperative Scheduling**: Main loop uses non-blocking updates across all managers with 10ms delay

### Key Components

- **InputManager** (`include/input_manager.h`): Handles KY-040 rotary encoder with rotation and click callbacks
- **UIManager** (`include/ui/ui_manager.h`): Manages SH1106 OLED display and view switching
- **UIView System**: Base class (`include/ui/ui_view.h`) with concrete views for temperature monitoring and settings

### Hardware Integration

- Dual K-type thermocouples via MAX6675 SPI modules
- PWM-controlled MOSFET (IRLZ44N) for 5V blower fan speed
- SG90 servo for damper positioning
- 1.3" SH1106 OLED display (128x64) with I2C
- KY-040 rotary encoder for user input

### File Organization

- Headers in `include/` with implementation in `src/`
- Modular structure: `control/`, `sensors/`, `ui/`, `network/` subsystems
- Views inherit from UIView base class for consistent interface
- PlatformIO configuration uses Arduino framework with Adafruit display libraries

### Development Notes

- All timing should be non-blocking (millis() based, no delay() calls)
- Input callbacks must be lightweight to avoid blocking main loop
- Pin assignments defined in wokwi.toml: encoder CLK=2, DT=3, SW=4
- Target platform is ESP32 with 115200 baud serial monitoring