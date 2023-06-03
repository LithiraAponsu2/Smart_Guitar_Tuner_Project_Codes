

int x[1000] = {0};  // store analog values
int temp;
float freq[4] = {};

float get_freq() {
  float frequency = 0;
  int totalSamples = 1000;
  int blockSize = totalSamples / 2;

  for (int i = 0; i < totalSamples; i++) {
    x[i] = analogRead(26);
    delayMicroseconds(119);
  }

  float y[totalSamples - 1]; // array to store the moving average
  for (int i = 0; i < totalSamples - 1; i++) {
    y[i] = 0; // Initializing all zero
  }

  int l = 2;
  float mean = 0; // store mean

  // Moving Average filter
  unsigned long start = millis(); // Start time
  for (int i = 0; i < totalSamples - 1; i++) {
    for (int j = 0; j < l; j++) {
      if (i > j) {
        y[i] = y[i] + x[i - j];
      }
    }
    mean = mean + y[i];
  }
  unsigned long end = millis(); // End time
  Serial.print("Moving Average time: ");
  Serial.println(end - start);

  // Subtracting mean
  for (int i = 0; i < totalSamples - 1; i++) {
    y[i] = y[i] - mean / (totalSamples - 1);
  }

  float blocks[blockSize];
  int no_block = 2;

  float ans_mean = 0;
  for (int j = 0; j < no_block; j++) {
    for (int i = 0; i < blockSize; i++) {
      blocks[i] = y[i + blockSize * j];
    }

    // Energy of signal
    float energy = 0;
    for (int i = 0; i < blockSize; i++) {
      energy = energy + blocks[i] * blocks[i];
    }

    // ACF
    float acf[blockSize];
    for (int i = 0; i < blockSize; i++) {
      acf[i] = 0; // Initializing all zero
    }

    start = millis(); // Start time
    for (int k = 0; k < blockSize; k++) {
      for (int i = 0; i < blockSize; i++) {
        if ((i + k) < blockSize)
          acf[k] = acf[k] + blocks[i] * blocks[i + k];
      }
      acf[k] = acf[k] / energy;
    }
    end = millis(); // End time
    Serial.print("ACF time: ");
    Serial.println(end - start);

    // First zero crossing
    int zero;
    start = millis(); // Start time
    for (int z = 0; z < blockSize - 1; z++) {
      if (acf[z] * acf[z + 1] < 0) {
        zero = z;
        break;
      }
    }
    end = millis(); // End time
    Serial.print("Zero crossing time: ");
    Serial.println(end - start);

    // First maxima after zero
    int maxima = zero;
    start = millis(); // Start time
    for (int k = zero + 1; k < blockSize - 1; k++) {
      if (acf[k] > acf[maxima]) {
        maxima = k;
      }
    }
    end = millis(); // End time
    Serial.print("Maxima time: ");
    Serial.println(end - start);

    float numerator = 8000;
    float ans = numerator / maxima;
    freq[j] = ans; // store frequency in array
  }

  // Finding the same frequencies
  start = millis(); // Start time
  for (int i = 0; i < 1; i++) {
    for (int j = i + 1; j < 2; j++) {
      if (freq[i] == freq[j]) {
        frequency = freq[i];
        break;
      }
    }
  }
  end = millis(); // End time
  Serial.print("Finding same frequencies time: ");
  Serial.println(end - start);

  return frequency;
}



void setup() { // put your setup code here, to run once:
  Serial.begin(115200);
  analogReadResolution(12); //Setting up the baud rate 
}

// float listen() { 
//   temp = analogRead(26);
//   if (temp > 2500) {
//     return get_freq();  // need to return
//   }
//   return 0.0;
// }

void El_tune(){
  int temp3 = 0;
  float temp2;
  
  while (temp3 == 0){
    temp = analogRead(26);
    if (temp > 2500) {
      temp2 = get_freq();
      if(70 < temp2 && temp2 < 80){  // under
        Serial.println("under");
        Serial.println(temp2);
      }
      else if(83 < temp2 && temp2 < 93){  // over
        Serial.println("over");
        Serial.println(temp2);
      }
      else if(80 <= temp2 && temp2 <= 83){  // if (80 < temp2 < 82) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        temp3 == 1;
      }
      else{
        Serial.println("Stray");
      }
    }
  }
  
  
}

void A_tune(){
  int temp3 = 0;
  float temp2;
  while (temp3 == 0){
    temp = analogRead(26);
    if (temp > 2500) {
      temp2 = get_freq();
      if(100 < temp2 && temp2 < 107){  // under
        Serial.println("under");
        Serial.println(temp2);
      }
      else if(111 < temp2 && temp2 < 114){  // over
        Serial.println("over");
        Serial.println(temp2);
      }
      else if(107 <= temp2 && temp2 <= 111){  // if (108 < temp2 < 110) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        temp3 == 1;
      }
      else{
        Serial.println("Stray");
      }
    }
  }
}

void D_tune(){
  int temp3 = 0;
  float temp2;
  while (temp3 == 0){
    temp = analogRead(26);
    if (temp > 2500) {
      temp2 = get_freq();
      if(135 < temp2 && temp2 < 144){  // under
        Serial.println("under");
        Serial.println(temp2);
      }
      else if(146 < temp2 && temp2 < 153){  // over
        Serial.println("over");
        Serial.println(temp2);
      }
      else if(144 <= temp2 && temp2 <= 146){  // if (145 < temp2 < 146) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        temp3 == 1;
      }
      else{
        Serial.println("Stray");
      }
    }
  }
}

void G_tune(){
  int temp3 = 0;
  float temp2;
  while (temp3 == 0){
    temp = analogRead(26);
    if (temp > 2500) {
      temp2 = get_freq();
      if(180 <= temp2 && temp2 <= 190){  // under
        Serial.println("under");
        Serial.println(temp2);
      }
      else if(196 < temp2 && temp2 < 205){  // over
        Serial.println("over");
        Serial.println(temp2);
      }
      else if(190 <= temp2 && temp2 <= 196){  // if (190 < temp2 < 195) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        temp3 == 1;
      }
      else{
        Serial.println("Stray");
      }
    }
  }
}

void B_tune(){
  int temp3 = 0;
  float temp2;
  while (temp3 == 0){
    temp = analogRead(26);
    if (temp > 2500) {
      temp2 = get_freq();
      if((235 < temp2 && temp2 < 241) || (115 < temp2 && temp2 < 122)){  // under
        Serial.println("under");
        Serial.println(temp2);
      }
      else if((243 < temp2 && temp2 < 250) || (124 < temp2 && temp2 < 130)){  // over
        Serial.println("over");
        Serial.println(temp2);
      }
      else if ((241 <= temp2 && temp2 <= 243) || (122 <= temp2 && temp2 <= 124)){  // if (242 < temp2 < 243) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        temp3 == 1;
      }
      else{
        Serial.println("Stray");
        Serial.println(temp2);
      }
    }
  }
}

void Eh_tune(){
  int temp3 = 0;
  float temp2;
  while (temp3 == 0){
    temp = analogRead(26);
    if (temp > 2500) {
      temp2 = get_freq();
      if((155 < temp2 && temp2 < 162) || (325 < temp2 && temp2 < 332)){  // under
        Serial.println("under");
        Serial.println(temp2);
      }
      else if((164 < temp2 && temp2 < 170) || (334 < temp2 && temp2 < 340)){  // over
        Serial.println("over");
        Serial.println(temp2);
      }
      else if ((162 <= temp2 && temp2 <= 164) || (332 <= temp2 && temp2 <= 334)){  // if (162 < temp2 < 164) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        temp3 == 1;
      }
      else{
        Serial.println("Stray");
        Serial.println(temp2);
      }
    }
  }
}

void loop() { // put your main code here, to run repeatedly:

  // print frequency
  // temp = analogRead(26);
  // if (temp > 2500) {
  //   Serial.println(get_freq());
  // }
  // El_tune();
  // A_tune();
  // D_tune();
  // G_tune();
  // B_tune();
  Eh_tune();


  
}