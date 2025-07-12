void drawTemperatureView() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print("Grill: ");
  display.print(getGrillTemp());
  display.print(" F");

  display.setCursor(0, 30);
  display.print("Target: ");
  display.print(getTargetTemp());
  display.display();
}
