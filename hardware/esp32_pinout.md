# 📌 ESP32 Dev Board Pinout Plan — GrillBit Project

This is a stable, long-term pin assignment plan for your ESP32-based kamado grill controller.

---

## 📋 Summary Table — ESP32 Pin Assignments

| ESP32 Pin | Function                     | Connected To                  | Notes                             |
|-----------|------------------------------|-------------------------------|-----------------------------------|
| GPIO 2    | Encoder CLK (A)             | Rotary Encoder CLK           | Input                             |
| GPIO 3    | Encoder DT (B)              | Rotary Encoder DT            | Input                             |
| GPIO 4    | Encoder SW (Button)         | Rotary Encoder SW            | Input w/ internal pullup          |
| GPIO 5    | Servo Control (PWM)         | Damper Servo                 | Uses `Servo.write()`              |
| GPIO 12   | Fan Control (PWM via MOSFET)| N-FET Gate / Fan GND         | PWM speed control                 |
| GPIO 13   | MAX6675 CS1                 | Thermocouple 1 CS            | SPI CS 1                          |
| GPIO 14   | MAX6675 CS2                 | Thermocouple 2 CS            | SPI CS 2                          |
| GPIO 18   | SPI CLK                     | MAX6675                      | Hardware SPI                      |
| GPIO 19   | SPI MISO                    | MAX6675                      | Hardware SPI                      |
| GPIO 21   | I2C SDA                     | OLED Display                 | Shared I2C bus                    |
| GPIO 22   | I2C SCL                     | OLED Display                 | Shared I2C bus                    |
| GPIO 23   | Optional Feature / Button   | —                             | Reserved                          |

---

## 🖥️ OLED Display (SSD1306 I2C)

- **VCC** → 3.3V
- **GND** → GND
- **SDA** → GPIO 21
- **SCL** → GPIO 22

---

## 🌀 Rotary Encoder (KY-040 or similar)

- **CLK (A)** → GPIO 2
- **DT (B)**  → GPIO 3
- **SW (btn)** → GPIO 4
- **VCC** → 3.3V
- **GND** → GND

---

## 🔥 Thermocouple (MAX6675 x1 or x2)

- **VCC** → 3.3V
- **GND** → GND
- **SCK** → GPIO 18 (shared)
- **CS**  → GPIO 13 (TC1), GPIO 14 (TC2 optional)
- **MISO** → GPIO 19 (shared)

---

## 🌬️ Fan (PWM via MOSFET)

- **PWM Control** → GPIO 12
- **Power** → External 5V (from USB or battery)
- **GND** → Shared with ESP32

Use a small **N-channel MOSFET board** (e.g., IRLZ44N) with:
- Gate: GPIO 12
- Drain: Fan GND
- Source: GND

---

## 🕹️ Servo (Air Damper)

- **Signal** → GPIO 5
- **VCC** → 5V (can come from USB if current <500mA)
- **GND** → GND

---

## ⚡ Power Supply

- **5V USB** powers:
  - ESP32 Dev board
  - Fan (via MOSFET)
  - Servo (if low-power)

Use external 5V regulator if servo/fan draw exceeds USB current limit.

---

## 🧩 Reserved / Optional Pins

| GPIO | Suggested Use             |
|------|---------------------------|
| 15   | Boot detection or extra button |
| 23   | Graph screen toggle or future input |

---

## 📌 Pin Usage Diagram (High-Level View)

```
            +-----------------------------+
            |         ESP32 Dev Board     |
            |                             |
            | GPIO  2 ◀── Encoder CLK     |
            | GPIO  3 ◀── Encoder DT      |
            | GPIO  4 ◀── Encoder SW      |
            |                             |
            | GPIO  5 ──▶ Servo           |
            | GPIO 12 ──▶ Fan PWM (MOSFET)|
            |                             |
            | GPIO 13 ◀── MAX6675 CS1     |
            | GPIO 14 ◀── MAX6675 CS2     |
            | GPIO 18 ──▶ SPI CLK         |
            | GPIO 19 ◀── SPI MISO        |
            |                             |
            | GPIO 21 ──▶ I2C SDA (OLED)  |
            | GPIO 22 ──▶ I2C SCL (OLED)  |
            |                             |
            | GND     ──┐ Shared Ground   |
            | 3.3V/5V ──┘ Power to modules|
            +-----------------------------+
```

---

This pinout plan is designed to remain **stable throughout your firmware and hardware development**. Let me know if you want a **printable version**, **Wokwi schematic**, or Fritzing diagram too!
