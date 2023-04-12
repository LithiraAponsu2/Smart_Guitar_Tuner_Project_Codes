// japan karayage ekai article ekai frequency ekak enwa harida dannaaa

#include <Arduino.h>
#include "arduinoFFT.h"

#define MIC 26 


const uint16_t samples = 1024;
const uint16_t samplingFrequency = 8000; 

int16_t wave[samples];
double vReal[samples];       
double vImag[samples];

arduinoFFT FFT = arduinoFFT(vReal, vImag, samples, samplingFrequency);         // Create FFT object

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {

  for (int i = 0; i < samples; i++) {
    wave[i] = analogRead(A0);
    // Serial.println(wave[i]);
    delay(1000/samplingFrequency);              
  }

  for (int i = 0; i < samples; i++) {
    vReal[i] = (wave[i] - 2048) * 3.3 / 4096.0; 
    vImag[i] = 0;
  }

  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);

  FFT.Compute(FFT_FORWARD);

  FFT.ComplexToMagnitude();

  double x = FFT.MajorPeak();

  Serial.println(x, 6);
  
  delay(2000);
  
}