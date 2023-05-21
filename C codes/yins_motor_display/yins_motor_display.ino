#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <CheapStepper.h>

CheapStepper Stepper(6, 7, 8, 9);
LiquidCrystal_I2C lcd(0x27, 16, 2);

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
  Serial.begin();
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
      break;
    case 2:
      lcd.print("A");
      break;
    case 3:
      lcd.print("D");
      break;
    case 4:
      lcd.print("G");
      break;
    case 5:
      lcd.print("B");
      break;
    case 6:
      lcd.print("Eh");
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


