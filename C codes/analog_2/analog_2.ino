#include <Arduino.h>
// #include <U8g2lib.h>
// #include <Wire.h>
// #include "arduinoFFT.h"

int micPin = 26; // connect microphone module to analog pin A0
int micValue = 0; // variable to store analog reading
int micArray[4094]; // array to store analog readings
float delay_time = 1000/8000;

void setup() {
  Serial.begin(9600); // initialize serial communication at 9600 bps
  analogReadResolution(12);
}

void loop() {
  for (int i = 0; i < 4094; i++) {
    micValue = analogRead(micPin); // read analog value from microphone module
    Serial.println(micValue);
    micArray[i] = micValue; 
    delay(delay_time); // wait for 10 milliseconds before taking the next reading
  }

  for (int i = 0; i < 4094; i++) {
    Serial.println(micArray[i]); // print micValue reading to serial monitor
  }

}