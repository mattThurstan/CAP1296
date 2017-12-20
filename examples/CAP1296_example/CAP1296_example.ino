/*
*Example sketch for..
*Arduino library for Microchip capacitive touch IC - CAP1296
*Uses I2C for communication.
*
*Based on CAP1298 library by Chris Ball
*https://github.com/ChrisBall/CAP1298
*/


#include <CAP1296.h>

CAP1296 touchSensor;

void setup() {
  Serial.begin(9600);
  touchSensor.begin();
}

void loop() {
  if (touchSensor.touchStatusChanged()) {
    touchSensor.updateTouchData();
    for (int i = 0; i < 6; i++) {
      if (touchSensor.isNewTouch(i)) {
        Serial.println("Touch sensor " + String(i) + " was just touched.");
      }
      if (touchSensor.isNewRelease(i)) {
        Serial.println("Touch sensor " + String(i) + " was just released.");
      }
    }
  }
  delay(50);
}