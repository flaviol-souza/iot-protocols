/*
 * Signal     Pin              Pin
 *            Arduino Uno      MFRC522 board
 */ -----------------------------------------
 // Reset      9                RST
 // SPI SS     10               SDA
 // SPI MOSI   11               MOSI
 // SPI MISO   12               MISO
 // SPI SCK    13               SCK


#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);  
void setup() {
    Serial.begin(9600); 
    SPI.begin();      
    mfrc522.PCD_Init(); 
    Serial.println("Scan tag to see UID...");
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}
