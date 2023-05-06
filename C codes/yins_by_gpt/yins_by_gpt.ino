#include <Arduino.h>

// YIN algorithm for pitch detection

int temp;

const int sampleRate = 8000;
const int bufferSize = 1024;

// YIN parameters
const int yinBufferLength = bufferSize / 2;
const int yinThreshold = 10; // % of maximum value

// Audio buffer
int audioBuffer[bufferSize];

// YIN buffer
float yinBuffer[yinBufferLength];

// Compute autocorrelation of the audio buffer
void autocorrelate() {
  for (int j = 0; j < yinBufferLength; j++) {
    float r = 0;
    for (int i = 0; i < bufferSize - j; i++) {
      r += audioBuffer[i] * audioBuffer[i + j];
    }
    yinBuffer[j] = r;
  }
}

// Compute YIN difference function
void difference() {
  for (int j = 1; j < yinBufferLength; j++) {
    float d = 0;
    for (int i = 0; i < yinBufferLength; i++) {
      float diff = audioBuffer[i] - audioBuffer[i + j];
      d += diff * diff;
    }
    yinBuffer[j] = d / ((bufferSize - j) * bufferSize);
  }
}

// Find first minimum of YIN difference function
int findMinimum() {
  int minIndex = 0;
  float minValue = 1.0f;

  for (int i = 1; i < yinBufferLength; i++) {
    if (yinBuffer[i] < minValue) {
      minValue = yinBuffer[i];
      minIndex = i;
    }
  }

  return minIndex;
}

// Compute fundamental frequency
float computeFundamentalFrequency(int minIndex) {
  float fundamentalFrequency = sampleRate / minIndex;
  return fundamentalFrequency;
}

void setup() {
  // initialize serial communication
  Serial.begin(115200);
  analogReadResolution(12);
}

void loop() {
  // Serial.println(analogRead(26));
  // delay(100);
  temp = analogRead(26);
  if (temp > 2150){

    // read in audio samples from ADC
    for (int i = 0; i < bufferSize; i++) {
      audioBuffer[i] = analogRead(26);
      delayMicroseconds(125);
    }

    // compute YIN difference function
    difference();

    // find first minimum of YIN difference function
    int minIndex = findMinimum();

    // compute fundamental frequency
    float fundamentalFrequency = computeFundamentalFrequency(minIndex);

    // output fundamental frequency
    Serial.println(fundamentalFrequency);

  }
}
