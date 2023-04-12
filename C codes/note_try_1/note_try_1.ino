// only guitar codes

#include <Arduino.h>
#include "arduinoFFT.h"

#define MIC 26 

const uint16_t samples = 1024;
const uint16_t samplingFrequency = 16000; 

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
    wave[i] = analogRead(MIC) - 1500;
    delay(1000/samplingFrequency);              
  }

  for (int i = 0; i < samples; i++) {
    vReal[i] = (wave[i] - 2048) * 3.3 / 4096.0; 
    vImag[i] = 0;
  }

  FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();

  double peakFreq = FFT.MajorPeak();
  Serial.print("Peak frequency: ");
  Serial.print(peakFreq);
  Serial.print(" Hz, corresponding to note ");

  if (peakFreq >= 78.00 && peakFreq < 82.41) {
    Serial.println("E_2");
  }
  else if (peakFreq >= 103.83 && peakFreq < 110.00) {
    Serial.println("A_2");
  }
  else if (peakFreq >= 130.81 && peakFreq < 138.59) {
    Serial.println("C_3");
  }
  else if (peakFreq >= 155.56 && peakFreq < 164.81) {
    Serial.println("E_3");
  }
  else if (peakFreq >= 196.00 && peakFreq < 207.65) {
    Serial.println("G_3");
  }
  else if (peakFreq >= 233.08 && peakFreq < 246.94) {
    Serial.println("B_3");
  }
  else {
    Serial.println("Unknown");
  }
  
  delay(2000);
}
