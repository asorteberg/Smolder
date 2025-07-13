# 🔥 SMOLDER ESP32 Kamado Grill Controller

This project is a Wi-Fi-connected controller for a Kamado-style grill, using an ESP32 microcontroller to manage:

- Two K-type thermocouples (for grill and food temp)
- A blower fan (for airflow control)
- A servo motor (for vent damper)
- A small OLED display and button panel
- A 3D-printed enclosure
- Panel-mount connectors for modular wiring
- Optional battery operation (5V USB recommended)

In contrast to previous open-source grill controllers, the primary objective here is create a solution that is cheaper then most of the commerical options while offering full customizibility. 

---

## 📦 Bill of Materials (BOM)


| Component                     | Part Number / Description             | Notes                                      |
|-------------------------------|----------------------------------------|--------------------------------------------|
| Microcontroller               | ESP32 Dev Module                       | Wi-Fi + Bluetooth, 3.3V logic               |
| Display                       | 1.3" SH1106 OLED.                      | 128x64 pixels, 3.3V/5V compatible           |
| Rotary Encoder + Button       | KY-040 Module                          | Includes pushbutton switch (SW pin)        |
| Fan                           | 5V DC Brushless Fan (e.g. 4010)        | For airflow control                         |
| Servo                         | SG90 Micro Servo                       | Controls damper                             |
| Thermocouple Amplifier        | MAX6675 Module                         | Reads K-type thermocouples                  |
| Thermocouple Jack             | 2.5mm Mono Panel Mount Jack (SJ1-2513) | For clean external thermocouple connections |
| Thermocouple Probe            | K-Type with 2.5mm Plug                 | Must be ungrounded for electrical safety    |
| MOSFET                        | IRLZ44N or IRL540N                     | Logic-level N-FET, used with custom board   |
| Passive Components            | 10kΩ, 220Ω Resistors, 1N5819 Diode     | For MOSFET pull-down, gate, and flyback     |
| Button Input (Optional)       | —                                      | Handled by encoder click                    |
| Enclosure                     | Custom 3D Printed                      | With cutouts for jack, USB, screen, encoder |


## ⚙️ Firmware Overview

### 🔌 Features
- Wi-Fi configuration portal (on first boot)
- Grill and food temperature reading via thermocouples
- Fan speed control via PWM + MOSFET
- Servo control for damper
- OLED screen display of temps and status
- Button interface for user settings (e.g. target temp)
- (Optional) Over-the-air (OTA) firmware updates

### 📦 Dependencies (Arduino/PlatformIO)

- [`WiFiManager`](https://github.com/tzapu/WiFiManager): Easy Wi-Fi provisioning
- `Adafruit_SSD1306` & `Adafruit_GFX`: OLED display
- `Servo.h`: PWM control of SG90
- `max6675`: SPI thermocouple reader
- `ESPAsyncWebServer` *(optional)*: If you want a built-in web UI
- `ArduinoJson`: For storing config/state in flash

---

## 🧠 MOSFET Usage

A **logic-level N-channel MOSFET** is used to switch the **5V fan**, controlled by the ESP32:

- **Recommended Part**: `IRLZ44N` or `AO3400`
- **Control via GPIO** (e.g. GPIO 26)
- **Flyback diode** recommended across the fan
- **Gate resistor (~220Ω)** and **pull-down resistor (~10kΩ)** improve signal stability
- Optional: use a **MOSFET breakout module** with all these parts preinstalled

### Example Fan PWM Setup
```cpp
const int FAN_PWM_PIN = 26;

void setup() {
  ledcSetup(0, 25000, 8);       // 25kHz PWM, 8-bit
  ledcAttachPin(FAN_PWM_PIN, 0);
}

void loop() {
  ledcWrite(0, 128);            // 50% speed (0–255)
}


## 🔄 Task Loop Model (Async-Cooperative)

This firmware uses a non-blocking, cooperative scheduling approach to avoid delays and ensure responsiveness across all subsystems.

/src/
│
├── main.cpp                      → App entry point, sets up tasks, loop dispatcher
│
├── config.h                      → Constants, pin mappings, default settings
│
├── ui/
│   ├── ui_manager.cpp/h         → View switching, rotary routing
│   ├── view_base.h              → Interface for screen views
│   ├── views/
│   │   ├── temp_view.cpp/h      → Temp display & setpoint adjustment
│   │   ├── settings_view.cpp/h  → Settings menu navigation
│   │   └── (future views...)    → GraphView, FanView, etc.
│   ├── encoder.cpp/h            → Rotary encoder and button logic (interrupt or polling)
│   └── display.cpp/h            → OLED screen setup & global draw helpers
│
├── sensors/
│   ├── thermocouple.cpp/h       → MAX6675 handling, smoothing, error states
│   └── eeprom.cpp/h             → Load/save settings (target temp, PID gains, etc.)
│
├── control/
│   ├── fan_control.cpp/h        → MOSFET speed control (PWM)
│   ├── servo_control.cpp/h      → Damper servo angle logic
│   ├── pid.cpp/h                → PID algorithm to compute output values
│   └── setpoint.cpp/h           → Handles temp targets and adjustment logic
│
├── network/
│   ├── wifi.cpp/h               → WiFiManager auto-connect, fallback AP mode
│   ├── web_server.cpp/h         → AsyncWebServer routes for remote status & control
│   └── ota.cpp/h                → ElegantOTA integration for browser firmware updates
│
├── utils/
│   ├── logger.cpp/h             → Serial logging macros (LOG_INFO, LOG_ERROR, etc.)
│   ├── scheduler.cpp/h          → Task manager using millis() or Ticker
│   └── math_utils.cpp/h         → Map, clamp, average helpers (for PID/input filtering)


