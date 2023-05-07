 
 //https://github.com/fdebrabander/Arduino-LiquidCrystal-I2C-library  
 #include <Wire.h> //This library allows you to communicate with I2C devices  
 //SDA -> GP4 and SCL -> GP5    
 #include <LiquidCrystal_I2C.h> //  This library is for character LCDs based on the HD44780 controller connected via I2C bus using the cheap I2C backpack modules based on the PCF8574 
 LiquidCrystal_I2C lcd(0x3F,16,2);  
 // if lcd is not print then use this 0x27..  
 //initialize the liquid crystal library
//the first parameter is the I2C address
//the second parameter is how many rows are on your display
//the third parameter is how many columns are on your display
 void setup()  
  {  
   lcd.begin();    //initialize lcd screen   
   lcd.backlight();  // turn on the backlight
   lcd.setCursor(0,0);  
   lcd.print("   Welcome to  ");  
   lcd.setCursor(0,1);  
   lcd.print("   Prasad Tech");  
   delay(3000);
  }  
 void loop()   
  {  
  lcd.clear();
   lcd.setCursor(0,0);  
   lcd.print("subscribe to  ");  
   lcd.setCursor(0,1);  
   lcd.print("    Prasad Tech");
   delay(3000);
  lcd.setCursor(0,0);  
   lcd.print("Technology Touch");  
   lcd.setCursor(0,1);  
   lcd.print("The Imagination");
   delay(3000);
  }  
