#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);   // I2C address: "0x3F" or "0x27"

const byte upButtonPin = 4;
const byte downButtonPin = 5;
const byte rightButtonPin = 6;
const byte leftButtonPin = 7;
const byte tweeterPin = 9;
const byte ledPin = 13;
const byte ledPin_fade = 10; 
int fade = 0;


String MenuItems[] = {  // Your menü items
  "LED Switch",
  "LED Brightness",
  "menuitem 3",
  "menuitem 4",
  "menuitem 5",
  "menuitem 6" 
  // and so on... 
};

void menuFunctions(int menu, byte right, byte left)  // Your menü functions
{
  if(menu == 1) // example function for 1. menü item
  {
    if(right == 1)
    {
      lcd.setCursor(0, 1);
      lcd.print("Off   ");
      lcd.setCursor(10, 1);
      lcd.print("On  <<");
      digitalWrite(ledPin, HIGH);
    }
    if(left == 1)
    {
      lcd.setCursor(0, 1);
      lcd.print("Off <<");
      lcd.setCursor(10, 1);
      lcd.print("On    ");
      digitalWrite(ledPin, LOW);
    }
  }
  if(menu == 2) // example function for 2. menü item
  {
    if(right == 1)
    {
      fade += 20;
      if(fade >= 255)
      {
        fade = 255;
      }
    }
    if(left == 1)
    {
      fade -= 20;
      if(fade <= 0)
      {
        fade = 0;
      }
    }
    lcd.setCursor(0, 1);
    lcd.print("Brightness:     ");
    lcd.setCursor(12, 1);
    lcd.print(fade);
    analogWrite(ledPin_fade, fade);
  }
  if(menu == 3)
  {
    lcd.setCursor(0, 1);
    lcd.print("Hello Menu Item 3");
  }
  // and so on... 
}


/***  do not modify  ***********************************************/
template< typename T, size_t NumberOfSize > 
size_t MenuItemsSize(T (&) [NumberOfSize]){ return NumberOfSize; }
int numberOfMenuItems = MenuItemsSize(MenuItems) - 1;
int currentMenuItem = 0;
int previousMenuItem = 1;
byte button_flag = 0;
unsigned long previousMillis = millis();
const int note = 4699;
void beepsOnce()
{
  tone(tweeterPin, note, 125);
  delay(60);
  noTone(tweeterPin);
}
/*******************************************************************/

void setup()
{
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin_fade, OUTPUT);

  /************************************/
  pinMode(upButtonPin, INPUT_PULLUP);
  pinMode(downButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);
  pinMode(leftButtonPin, INPUT_PULLUP);
  
  lcd.begin();
  // lcd.noBacklight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Simple");
  lcd.setCursor(3, 1);
  lcd.print("Dynamic Menu");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("myhomethings.eu");
  delay(5000);
  lcd.clear();
}

void loop() 
{
  if(digitalRead(rightButtonPin) == LOW && button_flag == 0)
  {
    menuFunctions(currentMenuItem + 1, 1, 0);
    button_flag = 1;
    previousMillis = millis();
    beepsOnce();
  }
  if(digitalRead(leftButtonPin) == LOW && button_flag == 0)
  {
    menuFunctions(currentMenuItem + 1, 0, 1);
    button_flag = 1;
    previousMillis = millis();
    beepsOnce();
  }
  if(digitalRead(upButtonPin) == LOW && button_flag == 0)
  {
    ++currentMenuItem;
    if(currentMenuItem > numberOfMenuItems )
    {
      currentMenuItem = numberOfMenuItems ;
    }
    button_flag = 1;
    previousMillis = millis();
    beepsOnce();
  }
  else if(digitalRead(downButtonPin) == LOW && button_flag == 0)
  {
    currentMenuItem--;
    if(currentMenuItem < 0)
    {
      currentMenuItem = 0;
    }
    button_flag = 1;
    previousMillis = millis();
    beepsOnce();
  }
  if(currentMenuItem != previousMenuItem)
  {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(MenuItems [currentMenuItem]);
    menuFunctions(currentMenuItem + 1, 0, 0);
    previousMenuItem = currentMenuItem;
  }
  if(millis() - previousMillis >= 400) 
  {
    previousMillis = millis();
    button_flag = 0;
  }
}