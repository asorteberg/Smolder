# 🔥 ESP32 Kamado Grill Controller

This project is a Wi-Fi-connected controller for a Kamado-style grill, using an ESP32 microcontroller to manage:

- Two K-type thermocouples (for grill and food temp)
- A blower fan (for airflow control)
- A servo motor (for vent damper)
- A small OLED display and button panel
- A 3D-printed enclosure
- Panel-mount connectors for modular wiring
- Optional battery operation (5V USB recommended)

---

## 📦 Bill of Materials (BOM)

| #  | Component                           | Description                                      | Part Number(s)                     | Alternatives                     |
|----|-------------------------------------|--------------------------------------------------|------------------------------------|----------------------------------|
| 1  | ESP32 Dev Board                     | ESP32-WROOM DevKit v1                            | ESP32-DEVKITC-32D                  | NodeMCU ESP32, WROOM-32U         |
| 2  | Thermocouple Probe (x2)            | K-type, M6 threaded, ~1m                         | K-01-M6-1000                       | Omega XCIB-K-M6-72               |
| 3  | MAX6675 Thermocouple Amplifier (x2)| SPI thermocouple interface                       | MAX6675 Module                     | MAX31855                         |
| 4  | SG90 Micro Servo                   | Controls damper position                         | TowerPro SG90                      | MG90S                            |
| 5  | Blower Fan                         | 5V, 30mm radial fan for airflow                  | 3010 Blower Fan 5V                 | 4010 Fan                         |
| 6  | OLED Display                       | 0.96" 128x64 I2C SSD1306                         | SSD1306 OLED                       | SH1106 OLED                      |
| 7  | Tactile Button Panel               | 3-button breakout or panel mount switches        | DFR0029 (or tact switch board)     | DIY tact switches                |
| 8  | Panel-Mount Thermocouple Jack (x2)| K-type screw terminal, mini flat panel mount     | PMP-K-M                            | Omega SMP-K-M                    |
| 9  | Panel USB Power Port               | USB-A or USB-C panel mount                       | USB-A Female Panel Mount           | USB-C Panel Mount                |
| 10 | Fan/Servo Connector                | 6-pin GX12 aviation connector                    | GX12-6                             | JST-XH 6-pin                     |
| 11 | Battery Holder + Boost             | 18650 holder with 5V boost module                | MT3608 + holder                    | TP4056 module                    |
| 12 | Wiring Kit                         | Dupont cables or JST harness                     | Jumper wire kit                    | JST pigtails                     |
| 13 | N-Channel MOSFET Breakout         | Logic-level with pull-down and gate resistors    | IRLZ44N / AO3400 Module            | AO3401, STP55NF06L               |
| 14 | Flyback Diode (optional)          | Across fan terminals                             | 1N5819                             | 1N4007                           |
| 15 | 3D-Printed Enclosure              | Designed in Fusion 360                           | –                                  | Self-printed                     |

---

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

```cpp
void setup() {
  initSerial();
  initWiFi();
  initWebServer();
  initDisplay();
  initEncoder();
  initSensors();
  initActuators();

  scheduler.addTask("readTemps", 1000, readTemperatures);
  scheduler.addTask("updateUI", 500, drawUI);
  scheduler.addTask("fanControl", 500, updateFan);
}

void loop() {
  scheduler.run();        // Check and run timed tasks
  checkRotaryInput();     // Poll encoder
  handleWiFi();           // Reconnect if needed
}

| Design Choice                       | Benefit                                             |
| ----------------------------------- | --------------------------------------------------- |
| 🧩 **Modular folders**              | Each responsibility is isolated and testable        |
| ⚙️ **Ticker/task system**           | Avoids `delay()` and blocking code                  |
| 🛡️ **Error handling per module**   | Sensor read failures don’t crash system             |
| 🧠 **Preferences**                  | Config state stored in flash, auto-reloaded         |
| 🌐 **Async Web Server**             | Doesn’t block UI/sensors                            |
| 🧼 **Non-blocking display updates** | UI refreshes when data changes, not just on a timer |

