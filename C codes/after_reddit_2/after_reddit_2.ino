// only get samples after major spike occurd and get 2048 samples

#include <Arduino.h>
#include "arduinoFFT.h"

#define MIC 26

const int samples = 4096;
const int samplingFrequency = 8000;

int temp;
double mean;

float average(int values[], int length) {
  float sum = 0;
  for (int i = 0; i < length; i++) {
    sum += values[i];
  }
  return sum / length;
}

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

}



void loop() {

  temp = analogRead(MIC);
  if (temp > 2150){

    int wave[samples] = {0};
    double vReal[samples] = {0};       
    double vImag[samples] = {0};

    arduinoFFT FFT = arduinoFFT(vReal, vImag, samples, samplingFrequency);

    for (int i = 0; i < samples; i++) {
      wave[i] = analogRead(MIC);
      // Serial.println(wave[i]);
      delayMicroseconds(125);             
    }

    mean = int(average(wave, samples));

    for (int i = 0; i < samples; i++) {
      vReal[i] = wave[i] - mean; 
      // Serial.print(vReal[i]);
      // Serial.print(", ");
                  
    }

    // Serial.println();

    FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);

    FFT.Compute(FFT_FORWARD);

    FFT.ComplexToMagnitude();

    double x = FFT.MajorPeak();

    Serial.println(x, 2);

  }

  



}
