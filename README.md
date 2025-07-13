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

