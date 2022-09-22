/*
   Signal     Pin              Pin
              Arduino Uno      MFRC522 board
   -----------------------------------------
   Reset      9                RST
   SPI SS     10               SDA
   SPI MOSI   11               MOSI
   SPI MISO   12               MISO
   SPI SCK    13               SCK
*/

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER 8
#define LED_VERMELHO 7
#define LED_AMARELO 6
#define LED_VERDE 5

MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.println("System Test");
  tone(BUZZER, 1500);
  digitalWrite(LED_VERDE, HIGH);
  digitalWrite(LED_VERMELHO, HIGH);
  digitalWrite(LED_AMARELO, HIGH);
  delay(3000);
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);
  digitalWrite(LED_AMARELO, LOW);
  noTone(BUZZER);

  digitalWrite(LED_AMARELO, HIGH);
  Serial.println("Scan PICC to see UID and type...");
}

void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  
  digitalWrite(LED_AMARELO, HIGH);
  Serial.print("UID da tag :");
  
  String conteudo = "";
  byte letra;
  for (byte i = 0; i < mfrc522.uid.size; i++)  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    conteudo.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    conteudo.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  
  Serial.println();
  Serial.print("Mensagem : ");
  conteudo.toUpperCase();
  if (conteudo.substring(1) == "ED 78 03 CA"
      || conteudo.substring(1) == "BD 9B 06 7D") {
    permitido();
  }

  negado();
  pinMode(LED_AMARELO, INPUT);
}

void permitido() {
  int qtd_bips = 2; 
  for (int j = 0; j < qtd_bips; j++) {
    tone(BUZZER, 1500);
    digitalWrite(LED_VERDE, HIGH);
    delay(100);

    noTone(BUZZER);
    digitalWrite(LED_VERDE, LOW);
    delay(100);
  }
}

void negado() {
  int qtd_bips = 1;
  for (int j = 0; j < qtd_bips; j++) {
    tone(BUZZER, 500);
    digitalWrite(LED_VERMELHO, HIGH);
    delay(500);

    noTone(BUZZER);
    digitalWrite(LED_VERMELHO, LOW);
    delay(500);
  }
}
