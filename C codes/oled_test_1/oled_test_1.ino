#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display = Adafruit_SSD1306(128, 32, &Wire);
byte compteur;
void setup() {
 	Serial.begin(9600);
 	Serial.println("OLED intialized");
 	display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
 	// text display tests
 	display.setTextSize(1);
 	display.setTextColor(WHITE);
}
void loop() {
 	for(compteur = 0; compteur = 25; compteur++) {
 			display.clearDisplay();
 			display.setCursor(0, 0);
 			display.println("test comr");
 			display.print("compteur: ");
 			display.println(compteur);
 			display.display();
 			delay(1000);
 	}
}