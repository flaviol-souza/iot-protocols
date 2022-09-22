#include <LiquidCrystal.h>
 
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
 
void setup()
{
  lcd.begin(16, 2);
}
 
void loop()
{
  lcd.clear();
  lcd.setCursor(3, 0);
  lcd.print("IFSP - CTD ");
  lcd.setCursor(3, 1);
  lcd.print(" Pos IoT");
  delay(5000);
   
  for (int posicao = 0; posicao < 3; posicao++)
  {
    lcd.scrollDisplayLeft();
    delay(300);
  }
   
  for (int posicao = 0; posicao < 6; posicao++)
  {
    lcd.scrollDisplayRight();
    delay(300);
  }
}
