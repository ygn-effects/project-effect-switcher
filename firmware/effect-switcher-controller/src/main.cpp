#include <Arduino.h>
#include "hal/hardware.h"

Hardware hardware;

void setup()
{
  Serial.begin(115200);
  hardware.setup();
  delay(500);
  hardware.startup();
  delay(500);
}

void loop()
{
  hardware.poll();

  hardware.resetTriggers();
}