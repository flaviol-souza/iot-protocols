#include <LiquidCrystal.h>

const int ledPin = 2;

// ---------  LCD [RS, EN, D4, D5, D6, D7 ]
LiquidCrystal lcd (13, 12, 14, 27, 26, 25);
 
void setup()
{
  pinMode(ledPin, OUTPUT);
  lcd.clear();
  lcd.print("Interfacing LCD");

  lcd.setCursor(0,1); 
  lcd.print ("with ESP32");
}
 
void loop()
{
  lcd.clear();
  digitalWrite(ledPin, HIGH);  
  delay(1000);                  
  digitalWrite(ledPin, LOW);
  lcd.print("Hello World!");
  delay(1000);
}
