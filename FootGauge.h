#ifndef PARTIALGAUGE_H
#define PARTIALGAUGE_H

#include <Arduino.h>
#include <MicroView.h>

class FootGauge : public MicroViewGauge {
 public: 
  FootGauge(uint8_t newx, uint8_t newy, int16_t min, int16_t max);
  void draw() override;
  void drawFace() override;
};

#endif
