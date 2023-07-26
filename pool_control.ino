#include <Arduino.h>
#include "PoolPumpController.h"

PoolPumpController poolPumpController;

void setup() {
  poolPumpController.Init();
}

void loop() {
  poolPumpController.Loop();
}
