// japan karayage eken mn gahapu ekak

#include <Arduino.h>
#include "arduinoFFT.h"

#define MIC 26 

arduinoFFT FFT = arduinoFFT();          // Create FFT object

const uint16_t samples = 128;
int16_t wave[samples];
double vReal[samples];       
double vImag[samples];

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {

  for (int i = 0; i < samples; i++) {
    wave[i] = analogRead(MIC);
    delayMicroseconds(21);              
  }

  for (int i = 0; i < samples; i++) {
    vReal[i] = (wave[i] - 2048) * 3.3 / 4096.0; 
    vImag[i] = 0;
  }

  FFT.Windowing(vReal, samples, FFT_WIN_TYP_HAMMING, FFT_FORWARD);

  FFT.Compute(vReal, vImag, samples, FFT_FORWARD); 

  FFT.ComplexToMagnitude(vReal, vImag, samples);

  for (int i = 0; i < samples / 2; i++) {
  Serial.print((i * 1.0 * 44100) / samples);
  Serial.print(" Hz: ");
  Serial.println(vReal[i]);
}



}