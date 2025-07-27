#include <Arduino.h>
#include "../../include/control/servo_control.h"

ServoController damperServo;

void setup() {
  Serial.begin(115200);
  Serial.println("Servo Component Test Starting...");
  
  damperServo.begin(5);
  Serial.println("Servo initialized on pin 5");
  
  Serial.println("Starting servo back and forth test");
}

void loop() {
  Serial.println("Moving servo to 0 degrees (closed)");
  damperServo.setAngle(0);
  delay(2000);
  
  Serial.println("Moving servo to 45 degrees (25% open)");
  damperServo.setAngle(45);
  delay(2000);
  
  Serial.println("Moving servo to 90 degrees (50% open)");
  damperServo.setAngle(90);
  delay(2000);
  
  Serial.println("Moving servo to 135 degrees (75% open)");
  damperServo.setAngle(135);
  delay(2000);
  
  Serial.println("Moving servo to 180 degrees (fully open)");
  damperServo.setAngle(180);
  delay(2000);
  
  Serial.println("Testing percentage-based positioning:");
  
  for (int i = 0; i <= 100; i += 25) {
    Serial.printf("Setting position to %d%%\n", i);
    damperServo.setPosition(i);
    Serial.printf("Current angle: %d degrees\n", damperServo.getCurrentAngle());
    delay(1500);
  }
  
  Serial.println("Smooth sweep test:");
  for (int angle = 0; angle <= 180; angle += 5) {
    damperServo.setAngle(angle);
    delay(100);
  }
  
  for (int angle = 180; angle >= 0; angle -= 5) {
    damperServo.setAngle(angle);
    delay(100);
  }
  
  Serial.println("Test cycle complete. Repeating...\n");
  delay(1000);
}