#include <Arduino.h>
// #include <U8g2lib.h>
// #include <Wire.h>
// #include "arduinoFFT.h"

int micPin = 15; // connect microphone module to analog pin A0
int micValue = 0; // variable to store analog reading
int micArray[4094]; // array to store analog readings


void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 bps
  analogReadResolution(12);
}

void loop() {
  micValue = analogRead(micPin); // read analog value from microphone module
  Serial.println(micValue); // print analog value to serial monitor
  delay(100); // wait for 10 milliseconds before taking the next reading
}
