#include <Arduino.h>
#include "hal/hardware.h"

Hardware hardware;

void setup()
{
  Serial.begin(115200);
  delay(500);
  hardware.setup();
  hardware.startup();
}

void loop()
{
  hardware.poll();
  hardware.process();
  hardware.resetTriggers();
}