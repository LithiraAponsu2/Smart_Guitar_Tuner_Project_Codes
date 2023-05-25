#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <CheapStepper.h>
#include <math.h>

CheapStepper Stepper(6, 7, 8, 9);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int x[4000] = {0};  // store analog values
int temp;
float freq[4] = {};

int buzzer = 16;
int upButton = 10;
int downButton = 11;
int selectButton = 12;  
int backButton = 13;  
int menu = 1;  // store pointed menu item
int tune_menu = 1;  // store pointed tune string 1-El, 2-A, 3-D, 4-G, 5-B, 6-Eh
int screen = 1;  // store menu screen 1 - main menu, 2 - string menu, 3 - Semi auto tune page, 4 - wind unwind page


byte upArrow[] = {
  B00000,
  B00000,
  B00100,
  B01110,
  B11111,
  B00000,
  B00000,
  B00000
};

byte downArrow[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B01110,
  B00100,
  B00000
};

byte tuneUpArrow[] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};

byte tuneDownArrow[] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B11111,
  B01110,
  B00100
};

byte allLight[] = {  // 3 width horizontal light
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110,
  B01110
};

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  lcd.begin();
  lcd.backlight();
  Stepper.setRpm(17);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP);
  updateTuneMenu();
  updateMenu();
  lcd.createChar(0, upArrow);
  lcd.createChar(1, downArrow);
  lcd.createChar(2, tuneUpArrow);
  lcd.createChar(3, tuneDownArrow);
  lcd.createChar(4, allLight);
}

void loop() {
  if (screen == 1) {
    // Menu screen
    if (!digitalRead(downButton)){
      menu++;
      updateMenu();
      delay(100);
      while (!digitalRead(downButton));  // avoid looping
    }
    if (!digitalRead(upButton)){
      menu--;
      updateMenu();
      delay(100);
      while(!digitalRead(upButton));
    }
    if (!digitalRead(selectButton)){
      executeActionMain();
      if (menu == 3) {
        screen = 2; // Set screen to TuneMenu when executing action3
      } else {
        updateMenu();
      }
      delay(100);
      while (!digitalRead(selectButton));
    }
  }
  if (screen == 2) {
    // TuneMenu screen
    if (!digitalRead(downButton)){
      tune_menu++;             
      updateTuneMenu();
      delay(100);
      while (!digitalRead(downButton));  // avoid looping
    }
    if (!digitalRead(upButton)){
      tune_menu--;
      updateTuneMenu();
      delay(100);
      while(!digitalRead(upButton));
    }
    if (!digitalRead(selectButton)){
      screen = 3;
      TunerPage();
      delay(100);
      while (!digitalRead(selectButton));
    }
    if (!digitalRead(backButton)){
      screen = 1;
      updateMenu();
      delay(100);
      while (!digitalRead(backButton));
    }
  }
  if (screen == 3) {
    // TuneMenu screen
    if (!digitalRead(backButton)){
      screen = 2;
      updateTuneMenu();
      delay(100);
      while (!digitalRead(backButton));
    }
  }
  if (screen == 4) {
    windUnwindPage();
    delay(100);
    while(true) {
      if (!digitalRead(downButton)){
        unWind();
      }
      if (!digitalRead(upButton)){
        wind();
      }
      if (!digitalRead(backButton)){
        screen = 1;
        updateMenu();
        delay(100);
        while (!digitalRead(backButton));
        break;
      }
    }
  }
}


void updateMenu() {
  switch (menu) {
    case 0:
      menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">Wind/Unwind");
      lcd.setCursor(0, 1);
      lcd.print(" Auto Tune");
      break;
    case 2:
      lcd.clear();
      lcd.print(" Wind/Unwind");
      lcd.setCursor(0, 1);
      lcd.print(">Auto Tune");
      break;
    case 3:
      lcd.clear();
      lcd.print(">Semi-Auto Tune");
      lcd.setCursor(0, 1);
      lcd.print(" Future");
      break;
    case 4:
      lcd.clear();
      lcd.print(" Semi-Auto Tune");
      lcd.setCursor(0, 1);
      lcd.print(">Future");
      break;
    case 5:
      menu = 4;
      break;
  }
}

void updateTuneMenu() {
  switch (tune_menu) {
    case 0:
      tune_menu = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print(">El A D G B Eh");
      break;
    case 2:
      lcd.clear();
      lcd.print(" El>A D G B Eh");
      break;
    case 3:
      lcd.clear();
      lcd.print(" El A>D G B Eh");
      break;
    case 4:
      lcd.clear();
      lcd.print(" El A D>G B Eh");
      break;
    case 5:
      lcd.clear();
      lcd.print(" El A D G>B Eh");
      break;
    case 6:
      lcd.clear();
      lcd.print(" El A D G B>Eh");
      break;
    case 7:
      tune_menu = 6;
      break;
    default:
      tune_menu = 1;  // Set tune_menu to 1 by default
      break;
  }
}

void windUnwindPage() {
  lcd.clear();
  lcd.createChar(0, upArrow);
  lcd.setCursor(0, 0);
  lcd.print("For Wind - ");
  lcd.write(0);
  lcd.setCursor(0, 1);
  lcd.print("For Unwind - ");
  lcd.write(1);
}

void executeActionMain() {
  switch (menu) {
    case 1:
      action1();
      break;
    case 2:
      action2();
      break;
    case 3:
      action3();
      break;
    case 4:
      action4();
      break;
  }
}

// actions of main menu
void action1() {
  lcd.clear();
  screen = 4;
  windUnwindPage();
}
void action2() {
  lcd.clear();
  lcd.print(">Auto Tune do");
  delay(1500);
}
void action3() {  // Semi-Auto
  lcd.clear();
  screen = 2;
  updateTuneMenu();
}
void action4() {
  lcd.clear();
  lcd.print(">Future Process");
  delay(1500);
}

// tuner page 
void TunerPage(){
  lcd.clear();
  lcd.setCursor(2, 0);
  lcd.write(4);
  lcd.setCursor(2, 1);
  lcd.write(4);
  lcd.setCursor(9, 1);
  lcd.write(2);
  lcd.setCursor(0, 0);
  switch (tune_menu) {
    case 1:
      lcd.print("El");
      El_tune();
      break;
    case 2:
      lcd.print("A");
      A_tune();
      break;
    case 3:
      lcd.print("D");
      D_tune();
      break;
    case 4:
      lcd.print("G");
      G_tune();
      break;
    case 5:
      lcd.print("B");
      B_tune();
      break;
    case 6:
      lcd.print("Eh");
      Eh_tune();
      break;
  }
}

void unWind() {
  Serial.println("CCW");
  Stepper.moveDegreesCCW(20);
}

void wind() {
  Serial.println("CW");
  Stepper.moveDegreesCW(20);
}

// frequency functions

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

void El_tune(){
  int temp3 = 0;
  float temp2;
  int tune_status = 0;
  
  while (temp3 == 0){
    temp = analogRead(26);
    if (!digitalRead(backButton)){
      screen = 2;
      updateTuneMenu();
      delay(100);
      while (!digitalRead(backButton));
      break;
    }
    if (temp > 2500) {
      temp2 = get_freq();
      if(70 < temp2 && temp2 < 80){  // under
        Serial.println("under");
        Serial.println(temp2);
        tune_status = -1;
        int point = floor((temp2 - 70) * 6/10);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("El");
        lcd.setCursor(3+point, 0);
        lcd.write(3);
        motor_tight();
      }
      else if(83 < temp2 && temp2 < 93){  // over
        Serial.println("over");
        Serial.println(temp2);
        tune_status = 1;
        int point = floor((temp2 - 83) * 6/10);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("El");
        lcd.setCursor(10+point, 0);
        lcd.write(3);
        motor_loose();
      }
      else if(80 <= temp2 && temp2 <= 83){  // if (80 < temp2 < 82) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        tune_status = 0;
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("El");
        lcd.setCursor(9, 0);
        lcd.write(3);
        tone(buzzer, 450);
        delay(500);
        noTone(buzzer);
        temp3 == 1;
      }
      else{
        Serial.println("Stray");
        Serial.println(temp2);
      }
    }
  }
  
  
}

void A_tune(){
  int temp3 = 0;
  float temp2;
  int tune_status = 0;
  while (temp3 == 0){
    temp = analogRead(26);
    if (!digitalRead(backButton)){
      screen = 2;
      updateTuneMenu();
      delay(100);
      while (!digitalRead(backButton));
      break;
    }
    if (temp > 2500) {
      temp2 = get_freq();
      if(100 < temp2 && temp2 < 107){  // under
        Serial.println("under");
        Serial.println(temp2);
        tune_status = -1;
        int point = floor((temp2 - 100) * 6/7);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("A");
        lcd.setCursor(3+point, 0);
        lcd.write(3);
        motor_tight();
      }
      else if(111 < temp2 && temp2 < 114){  // over
        Serial.println("over");
        Serial.println(temp2);
        tune_status = 1;
        int point = floor((temp2 - 111) * 6/3);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("A");
        lcd.setCursor(10+point, 0);
        lcd.write(3);
        motor_loose();
      }
      else if(107 <= temp2 && temp2 <= 111){  // if (108 < temp2 < 110) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        tune_status = 0;
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("A");
        lcd.setCursor(9, 0);
        lcd.write(3);
        tone(buzzer, 450);
        delay(500);
        noTone(buzzer);
        temp3 == 1;
      }
      else{
        Serial.println("Stray");
        Serial.println(temp2);
      }
    }
  }
}

void D_tune(){
  int temp3 = 0;
  float temp2;
  int tune_status = 0;
  while (temp3 == 0){
    temp = analogRead(26);
    if (!digitalRead(backButton)){
      screen = 2;
      updateTuneMenu();
      delay(100);
      while (!digitalRead(backButton));
      break;
    }
    if (temp > 2500) {
      temp2 = get_freq();
      if(135 < temp2 && temp2 < 144){  // under
        Serial.println("under");
        Serial.println(temp2);
        tune_status = -1;
        int point = floor((temp2 - 135) * 6/9);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("D");
        lcd.setCursor(3+point, 0);
        lcd.write(3);
        motor_tight();
      }
      else if(146 < temp2 && temp2 < 153){  // over
        Serial.println("over");
        Serial.println(temp2);
        tune_status = 1;
        int point = floor((temp2 - 146) * 6/7);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("D");
        lcd.setCursor(10+point, 0);
        lcd.write(3);
        motor_loose();
      }
      else if(144 <= temp2 && temp2 <= 146){  // if (145 < temp2 < 146) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        tune_status = 0;
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("D");
        lcd.setCursor(9, 0);
        lcd.write(3);
        tone(buzzer, 450);
        delay(500);
        noTone(buzzer);
        temp3 == 1;
      }
      else{
        Serial.println("Stray");
        Serial.println(temp2);
      }
    }
  }
}

void G_tune(){
  int temp3 = 0;
  float temp2;
  int tune_status = 0;
  while (temp3 == 0){
    temp = analogRead(26);
    if (!digitalRead(backButton)){
      screen = 2;
      updateTuneMenu();
      delay(100);
      while (!digitalRead(backButton));
      break;
    }
    if (temp > 2500) {
      temp2 = get_freq();
      if(180 <= temp2 && temp2 <= 190){  // under
        Serial.println("under");
        Serial.println(temp2);
        tune_status = -1;
        int point = floor((temp2 - 180) * 6/10);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("G");
        lcd.setCursor(3+point, 0);
        lcd.write(3);
        motor_tight();
      }
      else if(196 < temp2 && temp2 < 205){  // over
        Serial.println("over");
        Serial.println(temp2);
        tune_status = 1;
        int point = floor((temp2 - 196) * 6/9);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("G");
        lcd.setCursor(10+point, 0);
        lcd.write(3);
        motor_loose();
      }
      else if(190 <= temp2 && temp2 <= 196){  // if (190 < temp2 < 195) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        tune_status = 0;
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("G");
        lcd.setCursor(9, 0);
        lcd.write(3);
        tone(buzzer, 450);
        delay(500);
        noTone(buzzer);
        temp3 == 1;
      }
      else{
        Serial.println("Stray");
        Serial.println(temp2);
      }
    }
  }
}

void B_tune(){
  int temp3 = 0;
  float temp2;
  int tune_status = 0;
  while (temp3 == 0){
    temp = analogRead(26);
    if (!digitalRead(backButton)){
      screen = 2;
      updateTuneMenu();
      delay(100);
      while (!digitalRead(backButton));
      break;
    }
    if (temp > 2500) {
      temp2 = get_freq();
      if((235 < temp2 && temp2 < 241) || (115 < temp2 && temp2 < 122)){  // under
        Serial.println("under");
        Serial.println(temp2);
        tune_status = -1;
        int point = (235 < temp2 && temp2 < 241) ? floor((temp2 - 235) * 6/6) : floor((temp2 - 115) * 6/7);  
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("B");
        lcd.setCursor(3+point, 0);
        lcd.write(3);
        motor_tight();
      }
      else if((243 < temp2 && temp2 < 250) || (124 < temp2 && temp2 < 130)){  // over
        Serial.println("over");
        Serial.println(temp2);
        tune_status = 1;
        int point = (243 < temp2 && temp2 < 250) ? floor((temp2 - 243) * 6/7) : floor((temp2 - 124) * 6/6);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("B");
        lcd.setCursor(10+point, 0);
        lcd.write(3);
        motor_loose();
      }
      else if ((241 <= temp2 && temp2 <= 243) || (122 <= temp2 && temp2 <= 124)){  // if (242 < temp2 < 243) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        tune_status = 0;
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("B");
        lcd.setCursor(9, 0);
        lcd.write(3);
        tone(buzzer, 450);
        delay(500);
        noTone(buzzer);
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
  int tune_status = 0;
  while (temp3 == 0){
    temp = analogRead(26);
    if (!digitalRead(backButton)){
      screen = 2;
      updateTuneMenu();
      delay(100);
      while (!digitalRead(backButton));
      break;
    }
    if (temp > 2500) {
      temp2 = get_freq();
      if((155 < temp2 && temp2 < 162) || (325 < temp2 && temp2 < 332)){  // under
        Serial.println("under");
        Serial.println(temp2);
        tune_status = -1;
        int point = (155 < temp2 && temp2 < 162) ? floor((temp2 - 155) * 6/7) : floor((temp2 - 325) * 6/7);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("Eh");
        lcd.setCursor(3+point, 0);
        lcd.write(3);
        motor_tight();
      }
      else if((164 < temp2 && temp2 < 170) || (334 < temp2 && temp2 < 340)){  // over
        Serial.println("over");
        Serial.println(temp2);
        tune_status = 1;
        int point = (164 < temp2 && temp2 < 170) ? floor((temp2 - 164) * 6/6) : floor((temp2 - 334) * 6/6);
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("Eh");
        lcd.setCursor(10+point, 0);
        lcd.write(3);
        motor_loose();
      }
      else if ((162 <= temp2 && temp2 <= 164) || (332 <= temp2 && temp2 <= 334)){  // if (162 < temp2 < 164) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        tune_status = 0;
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.write(4);
        lcd.setCursor(2, 1);
        lcd.write(4);
        lcd.setCursor(9, 1);
        lcd.write(2);
        lcd.setCursor(0, 0);
        lcd.print("Eh");
        lcd.setCursor(9, 0);
        lcd.write(3);
        tone(buzzer, 450);
        delay(500);
        noTone(buzzer);
        temp3 == 1;
      }
      else{
        Serial.println("Stray");
        Serial.println(temp2);
      }
    }
  }
}

void motor_tight(){ // when under tuned
  Stepper.moveDegreesCW(40);
}

void motor_loose(){  // when over tuned
  Stepper.moveDegreesCCW(40);
}