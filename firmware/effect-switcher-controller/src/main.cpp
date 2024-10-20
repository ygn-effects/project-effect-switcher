#include <Arduino.h>
#include "hal/hardware.h"

Hardware hardware;

void setup()
{
  Serial.begin(115200);
  hardware.hardwareSetup();
  delay(500);
  hardware.hardwareStartup();
  delay(500);
}

void loop()
{
  hardware.hardwarePoll();


  hardware.resetHardwareTriggers();
}