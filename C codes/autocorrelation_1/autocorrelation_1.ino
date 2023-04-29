#define MIC 26

#define LENGTH 2048

int rawData[LENGTH];
int count;

// Sample Frequency in kHz
const float sample_freq = 8000/18.1;

int temp;
double mean;
int len = sizeof(rawData);
int i,k;
long sum, sum_old;
int thresh = 0;
float freq_per = 0;
byte pd_state = 0;


void setup() {
  Serial.begin(115200);
  analogReadResolution(12);

}



void loop() {

  Serial.println("Lisning==========");

  temp = analogRead(MIC);
  if (temp > 2150){
    for (int i = 0; i < len; i++) {
      rawData[i] = analogRead(MIC);
      // Serial.println(wave[i]);
      delayMicroseconds(125);             
    }
  }

  Serial.println("Calculating");

  sum = 0;
  pd_state = 0;
  int period = 0;
  for(i=0; i < len; i++)
  {
    // Autocorrelation
    sum_old = sum;
    sum = 0;
    for(k=0; k < len-i; k++) sum += (rawData[k]-2048)*(rawData[k+i]-2048)/4096;
    // Serial.println(sum);
    
    // Peak Detect State Machine
    if (pd_state == 2 && (sum-sum_old) <=0) 
    {
      period = i;
      pd_state = 3;
    }
    if (pd_state == 1 && (sum > thresh) && (sum-sum_old) > 0) pd_state = 2;
    if (!i) {
      thresh = sum * 0.5;
      pd_state = 1;
    }
  }
  // for(i=0; i < len; i++) Serial.println(rawData[i]);
  // Frequency identified in Hz
  if (thresh >100) {
    freq_per = sample_freq/period;
    Serial.println(freq_per);
  }
  count = 0;

  





}