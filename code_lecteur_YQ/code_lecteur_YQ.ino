/* Créé par Yvon Quémener le 23 mars 2023
Ce code a été créé à partir des exemples fournis à l'intérieur des bibliothèques des périphériques
Bibliothèques à télécharger pour exécuter le code:
https://github.com/miguelbalboa/rfid
https://github.com/johnrickman/LiquidCrystal_I2C

 * --------------------------------------------------------------------------------------------------------------------
 * Example to change UID of changeable MIFARE card.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * This sample shows how to set the UID on a UID changeable MIFARE card.
 * 
 * @author Tom Clement
 * @license Released into the public domain.
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 *
 * More pin layouts for other boards can be found here: https://github.com/miguelbalboa/rfid#pin-layout
 */


#include <SPI.h>
#include <MFRC522.h> //bibliothèque à ajouter
 #include <Wire.h> 
#include <LiquidCrystal_I2C.h> //bibliothèque à ajouter
LiquidCrystal_I2C lcd(0x3F,16,2);  // lcd(0x3F,16,2)

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
bool etat = 0; 
void setup() 
{
  pinMode(2, INPUT_PULLUP); // interrupteur momentané mis à gnd
  pinMode(3, OUTPUT); digitalWrite(4, LOW);//gnd additionnel
  pinMode(4, OUTPUT);   // signal de sortie vers la barre intelligente
  Serial.begin(9600);   // Initiate a serial communication
  lcd.init();
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Initialisation");
  lcd.setCursor(0,1);
  lcd.print("rfid_295_01.ino");
  delay (3000);
  lcd.clear();
}

void message() {
    Serial.println("Carte lue - acces permis");
    Serial.println();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.backlight();
    lcd.print("Carte lue...");
    lcd.setCursor(0,1); lcd.print("SOUS TENSION"); digitalWrite(4, HIGH);
    etat = 1;
    delay(1000);
}

void interrupteur() { //détection du bouton d'arret
int buttonState = digitalRead(2);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == 0) {
    lcd.setCursor(0,1); lcd.print("HORS TENSION"); digitalWrite(4, LOW);
    Serial.println("STOP");
    etat = 0;
    delay(1000);
    lcd.noBacklight();
  }
}

void loop() 
{
  interrupteur();
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
  // pour ajouter une nouvelle carte RFID, lire le port série et ajouter le nouveau UID

  if (content.substring(1) == "C5 41 22 53") //change here the UID of the card/cards that you want to give access
  { message();}
  if (content.substring(1) == "C4 A6 23 D5") //change here the UID of the card/cards that you want to give access
  { message();}
  if (content.substring(1) == "7B 52 24 D5") //change here the UID of the card/cards that you want to give access
  { message();}
} 
