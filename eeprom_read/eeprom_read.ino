/*
 * EEPROM Read
 *
 * Reads the value of each byte of the EEPROM and prints it
 * to the computer.
 * This example code is in the public domain.
 */

#include <EEPROM.h>

// start reading from the first byte (address 0) of the EEPROM
int address = 0;
int maxAddress = 100;
int maxChar = 12;
bool print = true;

void setup() {
  // initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
}

void loop() {
 while(print){ 
  for (int i = 0; i < maxAddress; i+= maxChar+1){
    Serial.print("Index ");
    Serial.print(EEPROM.read(i));
    Serial.print("           Mot ");

    for (int k = 1; k <= maxChar; k++){
      Serial.print((char) EEPROM.read(i+k));
      Serial.print(" ");
    }
    Serial.println("");
  }
  print=false;
 }
}