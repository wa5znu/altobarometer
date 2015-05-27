#include <MicroView.h>
#include "FootGauge.h"

FootGauge::FootGauge(uint8_t newx, uint8_t newy, int16_t min, int16_t max):
  MicroViewGauge(newx, newy, min, max, WIDGETSTYLE1 + WIDGETNOVALUE) {
  drawFace();                                     // bleh
}

void FootGauge::drawFace() {
  MicroViewGauge::drawFace();
  // clear area for value display
  uView.rectFill(posX - 26, posY + 8, 48, 16, 0, NORM);
}

void FootGauge::draw() {
  MicroViewGauge::draw();
  uView.setCursor(posX - 16, posY + 6);
  if (value >= 0) {
    uView.print(F(" "));
  }
  uView.print(value);
  uView.print(F(" ft "));
}
