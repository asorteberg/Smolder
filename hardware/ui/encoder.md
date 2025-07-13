| Label       | Purpose                     | Connect to ESP32            | Notes                        |
| ----------- | --------------------------- | --------------------------- | ---------------------------- |
| **GND**     | Ground                      | GND                         | Common ground                |
| **+ (VCC)** | Power for pull-up resistors | 3.3V or 5V                  | Use 3.3V on ESP32 for safety |
| **SW**      | Push-button switch          | GPIO 4 (or any digital pin) | Active LOW                   |
| **DT**      | Data (secondary phase B)    | GPIO 3                      | Quadrature signal            |
| **CLK**     | Clock (primary phase A)     | GPIO 2                      | Quadrature signal            |


| Encoder Pin | ESP32 Pin | Label in Code     |
| ----------- | --------- | ----------------- |
| GND         | GND       | —                 |
| VCC         | 3.3V      | —                 |
| SW          | GPIO 4    | `PIN_ENCODER_SW`  |
| DT          | GPIO 3    | `PIN_ENCODER_DT`  |
| CLK         | GPIO 2    | `PIN_ENCODER_CLK` |


+-----------------------------+
|        ROTARY ENCODER      |
|                             |
|   CLK   (Pin 1)  ---> GPIO 2|
|   DT    (Pin 2)  ---> GPIO 3|
|   SW    (Pin 3)  ---> GPIO 4|
|   GND   (Pin 4) --->   GND  |
|   VCC   (Pin 5) --->  3.3V  |
+-----------------------------+


If you get noisy signals, add INPUT_PULLUP in pinMode() or use external 10kΩ pull-up resistors.


```
void loop() {
  Serial.print("CLK: "); Serial.print(digitalRead(2));
  Serial.print("  DT: "); Serial.print(digitalRead(3));
  Serial.print("  SW: "); Serial.println(digitalRead(4));
  delay(100);
}
```