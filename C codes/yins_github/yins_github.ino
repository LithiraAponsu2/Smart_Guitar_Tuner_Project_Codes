int x[4000] = {
  0
};
int temp;
void setup() { // put your setup code here, to run once:
  Serial.begin(115200);
  analogReadResolution(12); //Setting up the baud rate 
}

void loop() { // put your main code here, to run repeatedly:

  temp = analogRead(26);
  if (temp > 3000) {

    for (int i = 0; i < 4001; i++) {
      x[i] = analogRead(26);
      delayMicroseconds(118);
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

      // Serial.print(100*x[i]); //plotting input x
      // Serial.print(',');
      // Serial.println(100*y[i]/l+200); //plotting output y
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
        // Serial.println(20*acf[k]); //plotting output y
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
      ans_mean += ans;
      Serial.println(ans);
    }

    Serial.print("final =");
    Serial.println(ans_mean / 4);
  }
}