// only guitar codes

#include <Arduino.h>
#include "arduinoFFT.h"

#define adcPin 26 
#define SCL_INDEX 0x00
#define SCL_TIME 0x01
#define SCL_FREQUENCY 0x02
#define SCL_PLOT 0x03

const uint16_t samples = 1024;
const uint16_t samplingFrequency = 8000; 

volatile int resultNumber;
int16_t wave[samples];
double vReal[samples];       
double vImag[samples];

arduinoFFT FFT = arduinoFFT(vReal, vImag, samples, samplingFrequency);         // Create FFT object

ISR (ADC_vect)
{
    vReal[resultNumber++] = ADC;
            
    if(resultNumber == samples)
    {
      ADCSRA = 0;  // turn off ADC
    }
}

void timer_setup(){
  // reset Timer 1
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  TCCR1B = bit (CS11) | bit (WGM12);  // CTC, prescaler of 8
  TIMSK1 = bit (OCIE1B);
  OCR1A = ((16000000 / 8) / samplingFrequency) -1;  // sampling frequency = 16/8/250 MHz =  8  KHz 
}

void adc_setup() {
  ADCSRA =  bit (ADEN) | bit (ADIE) | bit (ADIF);   // turn ADC on, want interrupt on completion
  ADCSRA |= bit (ADPS2);  // Prescaler of 16
  ADMUX = bit (REFS0) | (adcPin & 7);
  ADCSRB = bit (ADTS0) | bit (ADTS2);  // Timer/Counter1 Compare Match B trigger source
  ADCSRA |= bit (ADATE);   // turn on automatic triggering  
}

void zeroI() {
   for (uint16_t i = 0; i < samples; i++)
  {
    vImag[i] = 0.0; //Imaginary part must be zeroed in case of looping to avoid wrong calculations and overflows
  } 
}

void PrintVector(float *vData, uint16_t bufferSize, uint8_t scaleType)
{
  for (uint16_t i = 0; i < bufferSize; i++)
  {
    float abscissa;
    /* Print abscissa value */
    switch (scaleType)
    {
      case SCL_INDEX:
        abscissa = (i * 1.0);
  break;
      case SCL_TIME:
        abscissa = ((i * 1.0) / samplingFrequency);
  break;
      case SCL_FREQUENCY:
        abscissa = ((i * 1.0 * samplingFrequency) / samples);
  break;
    }
    Serial.print(abscissa, 6);
    if(scaleType==SCL_FREQUENCY)
      Serial.print("Hz");
    Serial.print(" ");
    Serial.println(vData[i], 4);
  }
  Serial.println();
}

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Ready");

  zeroI(); // clears imaginary data
  timer_setup();
  adc_setup();
  
}

void loop()
{
  // waits until the array is full
  while (resultNumber < samples){ }

  /* Print the raw sampled data */
  //  Serial.println("Data:");
  //  PrintVector(vReal, samples, SCL_TIME);
  
  FFT.dcRemoval(); 
  FFT.windowing(FFT_WIN_TYP_HAMMING, FFT_FORWARD);	// Weigh data
  FFT.compute(FFT_FORWARD); // Compute FFT
  FFT.complexToMagnitude(); // Compute magnitudes 
  // printing the spectrum and the fundamental frequency f0
  PrintVector(vReal, (samples >> 1), SCL_FREQUENCY);
  float x = FFT.majorPeak();
  Serial.print("f0=");
  Serial.print(x, 6);
  Serial.println("Hz");

  resultNumber = 0; 

// ADC could be started again
/*  delay(1000);
  zeroI();
  adc_setup();
*/
 
}
