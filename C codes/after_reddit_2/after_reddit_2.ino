// only get samples after major spike occurd and get 2048 samples

#include <Arduino.h>
#include "arduinoFFT.h"

// #define MIC 26  // for pico
#define MIC 15  // esp32
// int MIC = 15;

const int samples = 1024;
const int samplingFrequency = 8000/2.08;


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
  Serial.println(temp);
  delay(100);
  if (temp > 1800){  //  > 2150 pico

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
      Serial.print(vReal[i]);
      Serial.print(", ");
                  
    }

    Serial.println();

    FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);

    FFT.Compute(FFT_FORWARD);

    // FFT.ComplexToMagnitude();

    // double x = FFT.MajorPeak();

    // Serial.println(x, 2);

    for (int i = 0; i < samples; i++) {
      vReal[i] = pow(vReal[i], 2);
    }

    int binIndex50Hz = 50 * samples / samplingFrequency;
    int binIndex400Hz = 400 * samples / samplingFrequency;

    double maxPower = 0;
    int maxPowerIndex = 0;
    for (int i = binIndex50Hz; i <= binIndex400Hz; i++) {
      if (vReal[i] > maxPower) {
        maxPower = vReal[i];
        maxPowerIndex = i;
      }
    }

    double maxPowerFrequency = maxPowerIndex * samplingFrequency / samples;

    Serial.println(maxPowerFrequency, 2);

  }

  



}
