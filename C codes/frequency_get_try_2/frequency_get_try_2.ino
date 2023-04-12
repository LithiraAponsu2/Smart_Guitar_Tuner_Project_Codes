#include <arduinoFFT.h>

#define SAMPLE_RATE 44100
#define N_SAMPLES SAMPLE_RATE * 3

double samples[N_SAMPLES];
double frequency;
double max_power = 0;

arduinoFFT FFT = arduinoFFT(samples, N_SAMPLES, SAMPLE_RATE);

void setup() {
  Serial.begin(9600);
}

void loop() {
  // Record audio for 3 seconds
  for (int i = 0; i < N_SAMPLES; i++) {
    samples[i] = analogRead(A0) - 512;
    delayMicroseconds(1000000 / SAMPLE_RATE);
  }

  // Compute FFT
  FFT.Windowing(FFT_WIN_TYP_HAMMING);
  FFT.Compute(FFT_FORWARD);
  FFT.ComplexToMagnitude();

  // Find maximum power frequency
  for (int i = 0; i < N_SAMPLES / 2; i++) {
    if (samples[i] > max_power) {
      max_power = samples[i];
      frequency = i * SAMPLE_RATE / N_SAMPLES;
    }
  }

  Serial.print("Maximum power frequency: ");
  Serial.print(frequency);
  Serial.println(" Hz");

  // Reset max_power for the next iteration
  max_power = 0;
}
