#include <CheapStepper.h>

CheapStepper Stepper(2, 3, 4, 5);

void setup() {

  Stepper.setRpm(17);

}

void loop() {

  Stepper.moveDegreesCW(360);
  delay(1000);
  Stepper.moveDegreesCCW(360);
  delay(1000);
  

}
