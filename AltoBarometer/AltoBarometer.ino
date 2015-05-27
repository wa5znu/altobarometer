/*
 * Copyright 2015 Leigh L. Klotz, Jr. WA5ZNU <Leigh@WA5ZNU.org>
 *
 * Altimeter-Barometer -- shows barometric pressure and
 * height relative to when you turned it on.
 *
 * Units set to millibar and feet.  Change if you want.
 *
 * Uses SparkFun MicroView, BMP180 barometer breakout board
 * and 3.3v power supply.
 * MicroView 
 * -https://www.sparkfun.com/products/12923
 * BMP180 
 * - https://www.sparkfun.com/products/11824
 * - http://www.adafruit.com/products/1604
 * 3.3v power supply
 * - AMS1117-3.3V on eBay, 5 for $1.88. 
 *
 * You can also power the whole thing from 3.3v directly, since the
 * MicroView handles it.
*/

#include <MicroView.h>
#include "FootGauge.h"

MicroViewWidget *altimeterWidget;

double last_p;
double last_a;

extern double baseline;

void setup() {
  setup_bmp180();
  last_p = baseline;
  last_a = getAltitude();
  uView.begin();
  uView.clear(PAGE);

  altimeterWidget = new FootGauge(32, 23, -100, 100);
}

void loop() {
  {
    double a = getAltitude();
    altimeterWidget->setValue((a + last_a) / 2);
    last_a = a;
  }

  {
    double p = getPressure();
    display_pressure((p + last_p) / 2.0);
    last_p = p;
  }

  uView.display();

}

void display_pressure(double p) {
  int i = p;
  uView.setCursor(altimeterWidget->getX() - 26, altimeterWidget->getY() + 16);
  uView.print(p, 1);
  uView.print(F(" mb"));
}

