#include "yinacf.h"

// Initialize the YIN object
YinACF<float> yin;

// The caller is expected to call initYin() during program 
// initialization, or whenever sample rate of minimum frequency
// requirements change.
void initYin(float sampleRate, float minFreq) {
  unsigned w, tmax;
  w = (unsigned) ceil(sampleRate / minFreq);
  tmax = w;
  yin.build(w, tmax);
}

// Calculate the fundamental frequency from the input samples and store in outFrequencies
int getFundamentalFrequency(int n, float* inSamples, float* outFrequencies) {
  int i;
  for (i = 0; i < n; ++i) {
    outFrequencies[i] = yin.tick(inSamples[i]);
  }
  return 0;
}

// Sample rate and minimum frequency required for YIN initialization
const float SAMPLE_RATE = 44100.0; // Change this to match your actual sample rate
const float MIN_FREQ = 100.0; // Change this to match your actual minimum frequency

// Define the size of the audio buffer and the input pin for the audio signal (e.g. microphone)
const int BUFFER_SIZE = 1024; // Change this to match your desired buffer size
const int AUDIO_PIN = A0; // Change this to match your actual audio input pin

// Define the audio buffer and the output frequency buffer
float audioBuffer[BUFFER_SIZE];
float frequencyBuffer[BUFFER_SIZE];

void setup() {
  // Initialize the YIN object with the sample rate and minimum frequency
  initYin(SAMPLE_RATE, MIN_FREQ);

  // Initialize serial communication for debugging purposes
  Serial.begin(9600);
}

void loop() {
  // Read audio samples from input pin and store in audio buffer
  for (int i = 0; i < BUFFER_SIZE; i++) {
    audioBuffer[i] = analogRead(AUDIO_PIN);
  }

  // Calculate the fundamental frequency using YIN algorithm
  getFundamentalFrequency(BUFFER_SIZE, audioBuffer, frequencyBuffer);

  // Output the frequency values to the serial monitor
  for (int i = 0; i < BUFFER_SIZE; i++) {
    Serial.println(frequencyBuffer[i]);
  }
}
