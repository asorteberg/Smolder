int currentView = 0;
const int NUM_VIEWS = 5;

void loop() {
  int dir = readEncoder();
  if (dir == 1) currentView = (currentView + 1) % NUM_VIEWS;
  if (dir == -1) currentView = (currentView - 1 + NUM_VIEWS) % NUM_VIEWS;

  drawView(currentView);
}

void drawView(int viewIndex) {
  display.clearDisplay();
  switch (viewIndex) {
    case 0: drawTemperature(); break;
    case 1: drawSettings(); break;
    case 2: drawGraph(); break;
    case 3: drawNetwork(); break;
    case 4: drawAbout(); break;
  }
  display.display();
}
