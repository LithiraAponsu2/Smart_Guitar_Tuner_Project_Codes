

int x[4000] = {0};  // store analog values
int temp;
float freq[4] = {};

float get_freq(){
  float frequency = 0;
  for (int i = 0; i < 4001; i++) {
    x[i] = analogRead(26);
    delayMicroseconds(119);
  }

  float y[4000]; //array to store the moving average
  for (int i = 0; i < 4000; i++) {
    y[i] = 0; //Initializing all zero
  }
  int l = 2; // 
  float mean; // store mean
  //Moving Average filter
  for (int i = 0; i < 4000; i++) //for loop for 4000 times
  {
    for (int j = 0; j < l; j++) {
      if (i > j) {
        y[i] = y[i] + x[i - j];
      }
    }
    mean = mean + y[i];

  }
  //subtracting mean
  for (int i = 0; i < 4000; i++) {
    y[i] = y[i] - mean / 4000;
  }

  float blocks[1000];
  int no_block = 4;

  float ans_mean = 0;
  for (int j = 0; j < no_block; j++) {
    for (int i = 0; i < 1000; i++) {
      blocks[i] = y[i + 1000 * j];
    }
    //Energy of signal
    float energy = 0;
    for (int i = 0; i < 1000; i++) {
      energy = energy + blocks[i] * blocks[i];
    }

    //ACF
    float acf[1000];
    for (int i = 0; i < 1000; i++) {
      acf[i] = 0; //Initializing all zero
    }

    for (int k = 0; k < 1000; k++) {
      for (int i = 0; i < 1000; i++) {
        if ((i + k) < 1000)
          acf[k] = acf[k] + blocks[i] * blocks[i + k];
      }
      acf[k] = acf[k] / energy;
    }

    //first zero crossing
    int zero;
    for (int z = 0; z < 999; z++) {
      if (acf[z] * acf[z + 1] < 0) {
        zero = z;
        break;
      }
    }
    //first maxima after z
    int maxima = zero;
    for (int k = zero + 1; k < 999; k++) {
      if (acf[k] > acf[maxima]) {
        maxima = k;
      }
    }
    float numerator = 8000;
    float ans = numerator / maxima;
    freq[j] = ans;  // store frequency in array
    // ans_mean += ans;
    // Serial.println(ans);
  }

  // Serial.print("final =");
  // Serial.println(ans_mean / 4);

  // if 2 or more frequencies same then give that value otherwise 0
  
  for (int i = 0; i < 3; i++) {
    for (int j = i+1; j < 4; j++) {
      if (freq[i] == freq[j]) {
        frequency = freq[i];
        break;
      }
    }
  }
  
  return frequency;
}

void setup() { // put your setup code here, to run once:
  Serial.begin(115200);
  analogReadResolution(12); //Setting up the baud rate 
}

float listen() { 
  temp = analogRead(26);
  if (temp > 2500) {
    return get_freq();  // need to return
  }
}

void El_tune(){
  float temp2 = listen();
  int temp3 = 0;
  while (temp == 0){
    if(80 => temp2){  // under
      Serial.println("under");
    }
    else if(temp2 >= 82){  // over
      Serial.println("over");
    }
    else{  // if (80 < temp2 < 82) tuned
      Serial.println("tuned");
      temp3 == 1;
    }
  }
}

void A_tune(){
  float temp2 = listen();
  int temp3 = 0;
  while (temp == 0){
    if(108 => temp2){  // under
      Serial.println("under");
    }
    else if(temp2 >= 110){  // over
      Serial.println("over");
    }
    else{  // if (108 < temp2 < 110) tuned
      temp3 == 1;
    }
  }
}

void D_tune(){
  float temp2 = listen();
  int temp3 = 0;
  while (temp == 0){
    if(145 => temp2){  // under
      Serial.println("under");
    }
    else if(temp2 >= 146){  // over
      Serial.println("over");
    }
    else{  // if (145 < temp2 < 146) tuned
      temp3 == 1;
    }
  }
}

void G_tune(){
  float temp2 = listen();
  int temp3 = 0;
  while (temp == 0){
    if(190 => temp2){  // under
      Serial.println("under");
    }
    else if(temp2 >= 195){  // over
      Serial.println("over");
    }
    else{  // if (190 < temp2 < 195) tuned
      temp3 == 1;
    }
  }
}

void B_tune(){
  float temp2 = listen();
  int temp3 = 0;
  while (temp == 0){
    if(242 => temp2){  // under
      Serial.println("under");
    }
    else if(temp2 >= 243){  // over
      Serial.println("over");
    }
    else{  // if (242 < temp2 < 243) tuned
      temp3 == 1;
    }
  }
}

void Eh_tune(){
  float temp2 = listen();
  int temp3 = 0;
  while (temp == 0){
    if(162 => temp2){  // under
      Serial.println("under");
    }
    else if(temp2 >= 164){  // over
      Serial.println("over");
    }
    else{  // if (162 < temp2 < 164) tuned
      temp3 == 1;
    }
  }
}

void loop() { // put your main code here, to run repeatedly:

  // print frequency
  // temp = analogRead(26);
  // if (temp > 2500) {
  //   Serial.println(get_freq());
  // }
  El_tune();


  
}