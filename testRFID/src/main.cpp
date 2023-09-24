#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5
#define SS_PIN 10

byte readCard[4];

MFRC522 mfrc522(SS_PIN, RST_PIN);



void setup() {
  // Initiating
  SPI.begin(); // SPI bus
  mfrc522.PCD_Init(); // MFRC522
  Serial.begin(9600);
}

void loop() {

  if ( ! mfrc522.PICC_IsNewCardPresent()) { //If a new PICC placed to RFID reader continue
  return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { //Since a PICC placed get Serial and continue
  return;
  }

  Serial.print("Nouveau tag de carte : ");

  for (int i=0; i<4; i++){
        Serial.print(mfrc522.uid.uidByte[i], HEX);
        Serial.print(" ");
  }
  Serial.println("");

  
}

