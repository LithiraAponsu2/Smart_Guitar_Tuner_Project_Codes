// japan karayage ekai article ekai frequency ekak enwa harida dannaaa

#include <Arduino.h>
#include "arduinoFFT.h"
#include <math.h>

#define MIC 26 

const uint16_t samples = 8192;
const uint16_t samplingFrequency = 8000/2.075; 

float mean;
int temp;



void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
}

float average(int16_t values[], uint16_t length) {
  float sum = 0;
  for (int i = 0; i < length; i++) {
    sum += values[i];
  }
  return sum / length;
}

void loop() {

  int16_t wave[samples] = {0};
  double vReal[samples] = {0};       
  double vImag[samples] = {0};

  arduinoFFT FFT = arduinoFFT(vReal, vImag, samples, samplingFrequency);         // Create FFT object

  for (int i = 0; i < samples; i++) {
    wave[i] = analogRead(A0);
    // Serial.println(wave[i]);
    delayMicroseconds(125);             
  }

  mean = average(wave, samples);

  Serial.println();
  Serial.println("lisnning==================");
  delay(500);
  

  for (int i = 0; i < samples; i++) {
    temp = wave[i] - mean;
    if (temp > 150) {
        for (int j = i+1; j < i+2001 && j < samples; j++) {
            vReal[j] = wave[j] - mean;
            // Serial.print(vReal[j]);
            // Serial.print(", ");
        }
        break;
    }
  }

  for (int i = 0; i < samples; i++) {
    Serial.print(vReal[i]);
    Serial.print(",");
    // delay(1);
  }


  // FFT.Windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);

  // FFT.Compute(FFT_FORWARD);

  // FFT.ComplexToMagnitude();

  // double x = FFT.MajorPeak();

  // Serial.println(x, 2);
  
  delay(2000);
  
}