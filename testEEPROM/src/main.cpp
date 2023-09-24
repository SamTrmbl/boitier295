#include <Arduino.h>
#include <EEPROM.h>

int addresse = 0;
byte dataEEPROM;

const int maxChar = 12; //incluant le null de terminaison, donc maxChar-1 lettres
char nomRx[maxChar];   // le nom à recevoir à chaque RX

int indexRx = 0;
char charRx;

boolean newData = false;
boolean majPrint = true;



//3 tags max, 16 colonnes car 4 octets pour UID, 12 octets pour nom (11 lettres max). 
//Ou faire une lettre de moins pour avoir un identifiants (0 = vide, après carte 1,2,3,4,).
//Un Arduino neuf à une EEPROM remplie de 255, faire le sketch EEPROM_clear avant.



void setup() {

  Serial.begin(9600);

  Serial.println("<Arduino is ready>");




 
}


void loop() {

  if (majPrint){
    if(EEPROM.read(0)==0){
      Serial.println("Il n'y a aucun nom d'inscrit. Veuillez taper un nom");
      majPrint = false;
    }
    else{
      Serial.println("Voici les noms inscrits");
      Serial.println("");
      for (int i=0; i < (int) EEPROM.length(); i += maxChar+1){//ajouter un + 4 quand il y aura les tags
        if(EEPROM.read(i) != 0){//Un nom est incrit
          Serial.print("Nom #");
          Serial.print(EEPROM.read(i));
          Serial.print(" :  ");
          for (int k = 1; k < maxChar; k++){
            char pourAfficher = (char) EEPROM.read(i+k);
            if (pourAfficher != 0x0){
              Serial.print(pourAfficher);

            }
           
          }
          Serial.println("");
        }


      }
      Serial.println("Pour inscrire un nom, taper le maintenant");
      Serial.println("");
      majPrint = false;

    
    }
  }

  
  

  
    while (Serial.available() > 0 && newData == false) {//Un nom est inscrit.
        charRx = Serial.read();
        
        

        if (charRx != '\n') {          //newline est la fin du mot envoyé par le terminal
            nomRx[indexRx] = charRx;
            indexRx++;
            if (indexRx >= maxChar) {  //Si on est rendu au maximum de longueur, on reste 
                indexRx = maxChar - 1; // "sur place" en attendant de recevoir le \n
            }
        }
        else {
            nomRx[indexRx] = 0x0; // termine le string avec un null
            indexRx = 0;
            newData = true;
        }
    }

    if (newData == true) {
        int newIndex = 0;

        for (int i=0; i < (int) EEPROM.length(); i += maxChar+1){//cherche le premier ID "nul" de 0
          if (EEPROM.read(i)==0){//ID nul, on veut inscrire le nom ici
            break;
          }
          else{
            newIndex++;
          }
        }

        int newID = newIndex + 1; //La carte à la position 0 est le ID 1

        Serial.println("");
        Serial.print("Nouveau nom pour la carte #");
        Serial.print(newID);
        Serial.print(" est: ");
        Serial.println(nomRx);
        Serial.println("");
        Serial.println("");
        Serial.println("");



        for (int i = 0; i < maxChar; i++){   
          addresse = newIndex*13+i;      //Ajuster le plus 13 quand TAG
          if (i==0){
            EEPROM.write(addresse, newID); 
          }
          else{
          EEPROM.write(addresse, nomRx[i-1]);
          }

 
        }
 
        for (int i=0; i < maxChar; i++){//reset nomRX au cas où on nom soit plus court que le précédent
          nomRx[i]='\0';
        }

        newData = false;
        majPrint=true;
    }
 

  



}