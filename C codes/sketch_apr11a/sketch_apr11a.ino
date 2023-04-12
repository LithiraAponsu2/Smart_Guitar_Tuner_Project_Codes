#include <arduinoFFT.h> // include the ArduinoFFT library

#define SAMPLES 1024 // set the number of samples to use for the FFT
#define SAMPLING_FREQUENCY 20000 // set the sampling frequency in Hz

double vReal[SAMPLES]; // create an array to store the real part of the input signal
double vImag[SAMPLES]; // create an array to store the imaginary part of the input signal

arduinoFFT FFT = arduinoFFT(); // create an instance of the ArduinoFFT class

void setup() {
  Serial.begin(9600); // initialize the serial communication at 9600 baud
}

void loop() {
  Serial.println("Start listening...");

  // Listen to audio for 5 seconds
  unsigned long startMillis = millis();
  while (millis() - startMillis < 5000) {
    // Read analog input from microphone module
    for (int i = 0; i < SAMPLES; i++) {
      vReal[i] = analogRead(A0); // read analog input from pin A0 and store in the vReal array
      vImag[i] = 0; // set the imaginary part of the input signal to 0
      delayMicroseconds(100); // delay for a short period to allow the input signal to settle
    }
  }

  // Perform FFT on input data
  FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
  FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
  FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);

  // Find peak frequency
  double peak = 0;
  int peakIndex = 0;
  for (int i = 2; i < (SAMPLES/2); i++) {
    if (vReal[i] > peak) {
      peak = vReal[i];
      peakIndex = i;
    }
  }

  // Calculate frequency
  double frequency = peakIndex * (SAMPLING_FREQUENCY / SAMPLES);

  // Output frequency to serial monitor
  Serial.print("Frequency: ");
  Serial.print(frequency);
  Serial.println(" Hz");

  Serial.println("End listening...\n");
  delay(1000); // delay for a short period before listening again
}
