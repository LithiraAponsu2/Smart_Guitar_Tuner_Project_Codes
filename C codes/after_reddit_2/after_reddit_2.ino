// only get samples after major spike occurd and get 2048 samples

#define MIC 26

const int samples = 2048;
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
  
  int wave[samples] = {0};
  double vReal[samples] = {0};

  Serial.println();
  Serial.println("lisnning==================");
  delay(500);

  temp = analogRead(MIC);
  if (temp > 2150){
    for (int i = 0; i < samples; i++) {
      wave[i] = analogRead(MIC);
      // Serial.println(wave[i]);
      delayMicroseconds(125);             
    }
  }

  mean = int(average(wave, samples));

  for (int i = 0; i < samples; i++) {
    vReal[i] = wave[i] - mean; 
    Serial.print(vReal[i]);
    Serial.print(", ");
                 
  }

  delay(2000);


}
