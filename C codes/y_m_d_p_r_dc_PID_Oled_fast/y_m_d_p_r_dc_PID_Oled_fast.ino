#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <CheapStepper.h>
#include <math.h>

#include <SPI.h>
// #include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// CheapStepper Stepper(6, 7, 8, 9);
// LiquidCrystal_I2C lcd(0x27, 16, 2);

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
int screen = 1;  // store menu screen 1 - main menu, 2 - string menu, 3 - Semi Violin page, 4 - wind unwind page

// dc motor
const int A1A = 7; 
const int A1B = 6;
const int ENCA = 20;  // Yellow
const int ENCB = 21;  // White

int pos = 0;
int speed_pwm = 200;
long prevT = 0;
float eprev = 0;
float eintegral = 0;

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

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);
  // lcd.begin();
  // lcd.backlight();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.println("OLED intialized");

  display.setTextSize(2);
  display.setTextColor(WHITE);
  // Stepper.setRpm(17);
  pinMode(upButton, INPUT_PULLUP);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  pinMode(backButton, INPUT_PULLUP);
  updateTuneMenu();
  updateMenu();
  // lcd.createChar(0, upArrow);
  // lcd.createChar(1, downArrow);
  // lcd.createChar(2, tuneUpArrow);
  // lcd.createChar(3, tuneDownArrow);
  // lcd.createChar(4, allLight);
  pinMode(A1A, OUTPUT);
  pinMode(A1B, OUTPUT);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA), readEncoder, RISING);

  // pinMode(8, OUTPUT);
  // digitalWrite(A1A, HIGH);
}

void loop() {
  // PID_motor(1200);
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
      // lcd.clear();
      display.clearDisplay();
      display.setCursor(0, 0);
      // lcd.print(">Wind/Unwind");
      display.println(">Wind");
      // lcd.setCursor(0, 1);
      display.setCursor(0, 17);
      // lcd.print(" Violin");
      display.println(" Violin");
      display.display();
      break;
    case 2:
      // lcd.clear();
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(" Wind");
      display.setCursor(0, 17);
      display.println(">Violin");
      display.display();
      // lcd.print(" Wind/Unwind");
      // lcd.setCursor(0, 1);
      // lcd.print(">Violin");
      break;
    case 3:
      // lcd.clear();
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(">Guitar");
      display.setCursor(0, 17);
      display.println(" Ukulele");
      display.display();
      // lcd.print(">Semi-Violin");
      // lcd.setCursor(0, 1);
      // lcd.print(" Ukulele");
      break;
    case 4:
      // lcd.clear();
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(" Guitar");
      display.setCursor(0, 17);
      display.println(">Ukulele");
      display.display();
      // lcd.print(" Semi-Violin");
      // lcd.setCursor(0, 1);
      // lcd.print(">Ukulele");
      break;
    case 5:
      menu = 4;
      break;
  }
}

void updateTuneMenu() {
  // display.setTextSize(1);
  switch (tune_menu) {
    case 0:
      tune_menu = 1;
      break;
    case 1:
      // lcd.clear();
      // lcd.print(">El A D G B Eh");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(">El A D G  B Eh");
      display.display();
      break;
    case 2:
      // lcd.clear();
      // lcd.print(" El>A D G B Eh");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(" El>A D G  B Eh");
      display.display();
      break;
    case 3:
      // lcd.clear();
      // lcd.print(" El A>D G B Eh");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(" El A>D G  B Eh");
      display.display();
      break;
    case 4:
      // lcd.clear();
      // lcd.print(" El A D>G B Eh");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(" El A D>G  B Eh");
      display.display();
      break;
    case 5:
      // lcd.clear();
      // lcd.print(" El A D G>B Eh");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(" El A D G >B Eh");
      display.display();
      break;
    case 6:
      // lcd.clear();
      // lcd.print(" El A D G B>Eh");
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println(" El A D G  B>Eh");
      display.display();
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
  // lcd.clear();
  // lcd.createChar(0, upArrow);
  // lcd.setCursor(0, 0);
  // lcd.print("For Wind - ");
  // lcd.write(0);
  // lcd.setCursor(0, 1);
  // lcd.print("For Unwind - ");
  // lcd.write(1);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Wind  : ");
  // display.drawBitmap(0, 100, upArrow, 6, 8, SSD1306_WHITE);
  display.drawTriangle(104, 12, 112, 4, 96, 4, WHITE);
  display.setCursor(0, 17);
  display.println("Unwind: ");
  display.drawTriangle(104, 20, 112, 28, 96, 28, WHITE);
  display.display();

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
  // lcd.clear();
  display.clearDisplay();
  screen = 4;
  windUnwindPage();
}
void action2() {
  // lcd.clear();
  // lcd.print(">Violin do");
  display.clearDisplay();
  display.println(">Violin do");
  delay(1500);
}
void action3() {  // Semi-Auto
  // lcd.clear();
  display.clearDisplay();
  screen = 2;
  updateTuneMenu();
}
void action4() {
  // lcd.clear();
  // lcd.print(">Ukulele Process");
  display.clearDisplay();
  display.println(">Ukulele");
  delay(1500);
}

// tuner page 
void TunerPage(){
  // lcd.clear();
  display.clearDisplay();

  display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
  display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
  display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
  display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
  display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
  display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
  
  display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
  display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
  display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
  display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
  display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
  display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

  display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
  display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
  display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
  display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
  display.drawLine(25, 25, 125, 25, SSD1306_WHITE);

  // lcd.setCursor(2, 0);
  // display.setCursor(2, 0);
  // lcd.write(4);
  // lcd.setCursor(2, 1);
  // lcd.write(4);
  // lcd.setCursor(9, 1);
  // lcd.write(2);
  // lcd.setCursor(0, 0);
  display.setCursor(0, 0);
  switch (tune_menu) {
    case 1:
      // lcd.print("El");
      display.print("El");
      display.display();
      El_tune();
      break;
    case 2:
      // lcd.print("A");
      display.print("A");
      display.display();
      A_tune();
      break;
    case 3:
      // lcd.print("D");
      display.print("D");
      display.display();
      D_tune();
      break;
    case 4:
      // lcd.print("G");
      display.print("G");
      display.display();
      G_tune();
      break;
    case 5:
      // lcd.print("B");
      display.print("B");
      display.display();
      B_tune();
      break;
    case 6:
      // lcd.print("Eh");
      display.print("Eh");
      display.display();
      Eh_tune();
      break;
  }
}

void unWind() {
  Serial.println("CCW");
  // Stepper.moveDegreesCCW(20);
  // digitalWrite(6, LOW);
  // digitalWrite(7, LOW);
  // digitalWrite(8, LOW);
  // digitalWrite(9, LOW);
  motor_rot_wind_CCW();
}

void wind() {
  Serial.println("CW");
  // Stepper.moveDegreesCW(20);
  // digitalWrite(6, LOW);
  // digitalWrite(7, LOW);
  // digitalWrite(8, LOW);
  // digitalWrite(9, LOW);
  motor_rot_wind_CW();
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
      // display.fillTriangle(33, 12, 25, 4, 41, 4, WHITE);
      display.display();
      if(70 < temp2 && temp2 < 80){  // under
        Serial.println("under");
        Serial.println(temp2);
        tune_status = -1;
        float delta_f = temp2 - 70;
        int angle = (10-delta_f) * 40/10;
        // int point = floor(delta_f * 6/10);
        int point = floor(delta_f * 32/10);  // 40-8
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("El");
        display.fillTriangle(33+point, 12, 25+point, 4, 41+point, 4, WHITE);
        display.display();
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("El");
        // lcd.setCursor(3+point, 0);
        // lcd.write(3);
        
        // motor_tight(angle);
        // PID_motor(angle+1000);
        // controlMotor(20, 80);
        motorRotateDelay(1, 1000);
      }
      else if(83 < temp2 && temp2 < 93){  // over
        Serial.println("over");
        Serial.println(temp2);
        tune_status = 1;
        float delta_f = temp2 - 83;
        int angle = delta_f * 40/10;
        // int point = floor(delta_f * 6/10);
        int point = floor(delta_f * 32/10);display.clearDisplay();
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("El");
        display.fillTriangle(85+point, 12, 77+point, 4, 93+point, 4, WHITE);
        display.display();
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("El");
        // lcd.setCursor(10+point, 0);
        // lcd.write(3);
        // motor_loose(angle);
        motorRotateDelay(-1, 1000);
      }
      else if(80 <= temp2 && temp2 <= 83){  // if (80 < temp2 < 82) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        tune_status = 0;
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("El");
        // lcd.setCursor(9, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("El");
        display.fillTriangle(75, 16, 67, 8, 84, 8, WHITE);
        display.display();
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
        float delta_f = temp2 - 100;
        int angle = (7-delta_f) * 40/7;
        int point = floor(delta_f * 6/7);
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("A");
        // lcd.setCursor(3+point, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("A");
        display.fillTriangle(33+point, 12, 25+point, 4, 41+point, 4, WHITE);
        display.display();
        // motor_tight(angle);
        motorRotateDelay(1, 1000);
      }
      else if(111 < temp2 && temp2 < 114){  // over
        Serial.println("over");
        Serial.println(temp2);
        tune_status = 1;
        float delta_f = temp2 - 111;
        int angle = delta_f * 40/3;
        int point = floor(delta_f * 6/3);
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("A");
        // lcd.setCursor(10+point, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("A");
        display.fillTriangle(85+point, 12, 77+point, 4, 93+point, 4, WHITE);
        display.display();
        // motor_loose(angle);
        motorRotateDelay(-1, 1000);
      }
      else if(107 <= temp2 && temp2 <= 111){  // if (108 < temp2 < 110) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        tune_status = 0;
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("A");
        // lcd.setCursor(9, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("A");
        display.fillTriangle(75, 16, 67, 8, 84, 8, WHITE);
        display.display();
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
        float delta_f = temp2 - 135;
        int angle = (9-delta_f) * 40/9;
        int point = floor(delta_f * 6/9);
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("D");
        // lcd.setCursor(3+point, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("D");
        display.fillTriangle(33+point, 12, 25+point, 4, 41+point, 4, WHITE);
        display.display();
        // motor_tight(angle);
        motorRotateDelay(1, 1000);
      }
      else if(146 < temp2 && temp2 < 153){  // over
        Serial.println("over");
        Serial.println(temp2);
        tune_status = 1;
        float delta_f = temp2 - 146;
        int angle = delta_f * 40/7;
        int point = floor(delta_f * 6/7);
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("D");
        // lcd.setCursor(10+point, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("D");
        display.fillTriangle(85+point, 12, 77+point, 4, 93+point, 4, WHITE);
        display.display();
        // motor_loose(angle);
        motorRotateDelay(-1, 1000);
      }
      else if(144 <= temp2 && temp2 <= 146){  // if (145 < temp2 < 146) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        tune_status = 0;
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("D");
        // lcd.setCursor(9, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("D");
        display.fillTriangle(75, 16, 67, 8, 84, 8, WHITE);
        display.display();
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
        float delta_f = temp2 - 180;
        int angle = (9-delta_f) * 40/10;
        int point = floor(delta_f * 6/10);
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("G");
        // lcd.setCursor(3+point, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("G");
        display.fillTriangle(33+point, 12, 25+point, 4, 41+point, 4, WHITE);
        display.display();
        // motor_tight(angle);
        motorRotateDelay(1, 1000);
      }
      else if(196 < temp2 && temp2 < 205){  // over
        Serial.println("over");
        Serial.println(temp2);
        tune_status = 1;
        float delta_f = temp2 - 196;
        int angle = delta_f * 40/9;
        int point = floor(delta_f * 6/9);
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("G");
        // lcd.setCursor(10+point, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("G");
        display.fillTriangle(85+point, 12, 77+point, 4, 93+point, 4, WHITE);
        display.display();
        // motor_loose(angle);
        motorRotateDelay(-1, 1000);
      }
      else if(190 <= temp2 && temp2 <= 196){  // if (190 < temp2 < 195) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        tune_status = 0;
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("G");
        // lcd.setCursor(9, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("G");
        display.fillTriangle(75, 16, 67, 8, 84, 8, WHITE);
        display.display();
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
        float delta_f = (235 < temp2 && temp2 < 241) ? (temp2 - 235) : (temp2 - 115);  
        int angle = (235 < temp2 && temp2 < 241) ? floor((6-delta_f) * 40/6) : floor((7-delta_f) * 40/7);
        int point = (235 < temp2 && temp2 < 241) ? floor(delta_f * 6/6) : floor(delta_f * 6/7);  
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("B");
        // lcd.setCursor(3+point, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("B");
        display.fillTriangle(33+point, 12, 25+point, 4, 41+point, 4, WHITE);
        display.display();
        // motor_tight(angle);
        motorRotateDelay(1, 1000);
      }
      else if((243 < temp2 && temp2 < 250) || (124 < temp2 && temp2 < 130)){  // over
        Serial.println("over");
        Serial.println(temp2);
        tune_status = 1;
        float delta_f = (243 < temp2 && temp2 < 250) ? (temp2 - 243) : (temp2 - 124);
        int angle = (243 < temp2 && temp2 < 250) ? floor(delta_f * 40/7) : floor(delta_f * 40/6);
        int point = (243 < temp2 && temp2 < 250) ? floor(delta_f * 6/7) : floor(delta_f * 6/6);
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("B");
        // lcd.setCursor(10+point, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("B");
        display.fillTriangle(85+point, 12, 77+point, 4, 93+point, 4, WHITE);
        display.display();
        // motor_loose(angle);
        motorRotateDelay(-1, 1000);
      }
      else if ((241 <= temp2 && temp2 <= 243) || (122 <= temp2 && temp2 <= 124)){  // if (242 < temp2 < 243) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        tune_status = 0;
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("B");
        // lcd.setCursor(9, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("B");
        display.fillTriangle(75, 16, 67, 8, 84, 8, WHITE);
        display.display();
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
        float delta_f = (155 < temp2 && temp2 < 162) ? (temp2 - 155) : (temp2 - 325);
        int angle = (155 < temp2 && temp2 < 162) ? floor((7-delta_f) * 40/7) : floor((7-delta_f) * 40/7);
        int point = (155 < temp2 && temp2 < 162) ? floor(delta_f * 6/7) : floor(delta_f * 6/7);
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("Eh");
        // lcd.setCursor(3+point, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("Eh");
        display.fillTriangle(33+point, 12, 25+point, 4, 41+point, 4, WHITE);
        display.display();
        // motor_tight(angle);
        motorRotateDelay(1, 1000);
      }
      else if((164 < temp2 && temp2 < 170) || (334 < temp2 && temp2 < 340)){  // over
        Serial.println("over");
        Serial.println(temp2);
        tune_status = 1;
        float delta_f = (164 < temp2 && temp2 < 170) ? (temp2 - 164) : (temp2 - 334);
        int angle = (164 < temp2 && temp2 < 170) ? floor(delta_f * 40/6) : floor(delta_f * 40/6);
        int point = (164 < temp2 && temp2 < 170) ? floor(delta_f * 6/6) : floor(delta_f * 6/6);
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("Eh");
        // lcd.setCursor(10+point, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("Eh");
        display.fillTriangle(85+point, 12, 77+point, 4, 93+point, 4, WHITE);
        display.display();
        // motor_loose(angle);
        motorRotateDelay(-1, 1000);
      }
      else if ((162 <= temp2 && temp2 <= 164) || (332 <= temp2 && temp2 <= 334)){  // if (162 < temp2 < 164) tuned
        Serial.println("tuned");
        Serial.println(temp2);
        tune_status = 0;
        // lcd.clear();
        // lcd.setCursor(2, 0);
        // lcd.write(4);
        // lcd.setCursor(2, 1);
        // lcd.write(4);
        // lcd.setCursor(9, 1);
        // lcd.write(2);
        // lcd.setCursor(0, 0);
        // lcd.print("Eh");
        // lcd.setCursor(9, 0);
        // lcd.write(3);
        display.clearDisplay();
        display.drawLine(25, 15, 65, 15, SSD1306_WHITE);  // left lines
        display.drawLine(25, 16, 65, 16, SSD1306_WHITE);
        display.drawLine(25, 17, 65, 17, SSD1306_WHITE);
        display.drawLine(25, 18, 65, 18, SSD1306_WHITE);
        display.drawLine(25, 19, 65, 19, SSD1306_WHITE);
        display.drawLine(25, 20, 65, 20, SSD1306_WHITE);
        
        display.drawLine(85, 15, 125, 15, SSD1306_WHITE);  // right lines
        display.drawLine(85, 16, 125, 16, SSD1306_WHITE);
        display.drawLine(85, 17, 125, 17, SSD1306_WHITE);
        display.drawLine(85, 18, 125, 18, SSD1306_WHITE);
        display.drawLine(85, 19, 125, 19, SSD1306_WHITE);
        display.drawLine(85, 20, 125, 20, SSD1306_WHITE);

        display.drawLine(25, 21, 125, 21, SSD1306_WHITE);  // bottom lines
        display.drawLine(25, 22, 125, 22, SSD1306_WHITE);
        display.drawLine(25, 23, 125, 23, SSD1306_WHITE);
        display.drawLine(25, 24, 125, 24, SSD1306_WHITE);
        display.drawLine(25, 25, 125, 25, SSD1306_WHITE);
        display.setCursor(0, 0);
        display.print("Eh");
        display.fillTriangle(75, 16, 67, 8, 84, 8, WHITE);
        display.display();
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

// void motor_tight(int angle_){ // when under tuned
//   Stepper.moveDegreesCW(angle_);
//   digitalWrite(6, LOW);
//   digitalWrite(7, LOW);
//   digitalWrite(8, LOW);
//   digitalWrite(9, LOW);
// }

// void motor_loose(int angle_){  // when over tuned
//   Stepper.moveDegreesCCW(angle_);
//   digitalWrite(6, LOW);
//   digitalWrite(7, LOW);
//   digitalWrite(8, LOW);
//   digitalWrite(9, LOW);
// }

void motor_rot_wind_CW(){  // only use delay for rotation control
  digitalWrite(A1B, HIGH);
  digitalWrite(A1A, LOW);
  delay(50);  // ~ 45 degrees
  digitalWrite(A1A, LOW);
  digitalWrite(A1B, LOW);
}

void motor_rot_wind_CCW(){  // only use delay for rotation control
  digitalWrite(A1A, HIGH);
  digitalWrite(A1B, LOW);
  delay(50);  // ~ 45 degrees
  digitalWrite(A1A, LOW);
  digitalWrite(A1B, LOW);
}

// PID motor control
void readEncoder() {
  int b = digitalRead(ENCB);
  if (b>0) {
    pos++;
  }
  else {
    pos--;
  }
}

void setMotor(int dir, int pwmVal) {
  if (dir == 1){
    analogWrite(A1A, pwmVal);
    digitalWrite(A1B, LOW);
  }
  else if (dir == -1) {
    analogWrite(A1B, pwmVal);
    digitalWrite(A1A, LOW);
  }
  else {
    digitalWrite(A1A, LOW);
    digitalWrite(A1B, LOW);
  }
}

// delay based motor rotate
void motorRotateDelay(int dir, int time){
  if (dir == 1){
    // analogWrite(A1A, pwmVal);
    digitalWrite(A1A, HIGH);
    digitalWrite(A1B, LOW);
    delay(time);
    digitalWrite(A1A, LOW);
  }
  else if (dir == -1) {
    // analogWrite(A1B, pwmVal);
    digitalWrite(A1B, HIGH);
    digitalWrite(A1A, LOW);
    delay(time);
    digitalWrite(A1B, LOW);
  }
  else {
    digitalWrite(A1A, LOW);
    digitalWrite(A1B, LOW);
  }
}


// void PID_motor(int target){

//   while (true){
//   static int pidCount = 0;
//   pidCount++;
  
//   // Rest of the PID_motor code...
  
//   Serial.print("PID Count: ");
//   Serial.println(pidCount);

//   // PID constants
//   float kp = 1;
//   float kd = 0.025;
//   float ki = 0;

  
//   // time difference
//   long currT = micros();

//   float deltaT = ((float) (currT-prevT)/1.0e6);
//   prevT = currT;

//   // error
//   int e = pos - target;

//   // derivative
//   float dedt = (e-eprev)/(deltaT);

//   // integral
//   // eintegral = eintegral + e*deltaT;

//   // control signal
//   float u = kp*e + kd*dedt;

//   // motor power
//   float pwr = fabs(u);
  
//   if(pwr>255){
//     pwr = 255;
//   }

//   if(pwr<70){
//     pwr = 70;
//   }

//   // motor direction
//   int dir = 1;
//   if(u<0){
//     dir = -1;
//   }

//   // signal the motor
//   setMotor(dir, pwr);

//   // store prev error
//   eprev = e;

//   Serial.println("---------------");
//   Serial.print(target);
//   Serial.print(" ");
//   Serial.print(pos);
//   Serial.println();
//   Serial.println(pwr);}
  

  
// }

// // Motor control to reach the target position (angle or steps)
// void controlMotor(int target, int pwmPower) {
//   // Motor constants
//   int kp = 1;

//   // Motor direction
//   int dir = 1;
//   if (target < pos) {
//     dir = -1;
//   }

//   // Move towards the target position
//   while (pos != target) {
//     int error = target - pos;
//     int u = kp * error;

//     // Cap the control signal to stay within pwmPower range
//     if (u > pwmPower) {
//       u = pwmPower;
//     } else if (u < -pwmPower) {
//       u = -pwmPower;
//     }

//     // Set motor power and direction
//     setMotor(dir, abs(u));

//     // Read the encoder for position update
//     readEncoder();
//   }

//   // Stop the motor after reaching the target
//   setMotor(0, 0);
// }