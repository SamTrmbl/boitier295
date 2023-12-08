#include <Arduino.h>
#include <EEPROM.h>

int addresse = 0;


const int maxChar = 12; //incluant le null de terminaison, donc maxChar-1 lettres
char nomRx[maxChar];   // le nom à recevoir à chaque RX


boolean newData = false;
boolean majPrint = true;



//??(1024/colonnes) tags max, 16 colonnes car 4 octets pour UID, 12 octets pour nom (11 lettres max). 
//Ou faire une lettre de moins pour avoir un identifiants (0 = vide, après carte 1,2,3,4,).
//Un Arduino neuf à une EEPROM remplie de 255, faire le sketch EEPROM_clear avant.



void setup() {

  Serial.begin(9600);

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
      //Cette section pourrait être améliorée
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

  if (Serial.available() > 0) {
    //Les String() ont une fonction pour lire facilement un message du Serial Monitor
    String incomingString = Serial.readStringUntil('\n'); 

    incomingString.trim(); //oter le whitespace
    incomingString = incomingString.substring(0, maxChar-1); //Découper si trop long. -1 pour laisser la place pour \0

    //Conversion du String en char array pour une meilleure intéraction avec le EEPROM
    memset(nomRx, '\0', maxChar); //reset le char array, utile si le dernier nom est plus long que le prochain
    incomingString.toCharArray(nomRx,maxChar);
    newData = true; 
  }



    if (newData == true) {
       //Cette section pourrait être améliorée !!
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


        //Inscrire les informations dans le EEPROM
        //La méthode put() gère tous les types de données automatiquement, contrairement à write() qui ne gère que byte
        addresse = newIndex*13;
        EEPROM.put(addresse, newID);
        EEPROM.put(addresse+1, nomRx); 


        newData = false;
        majPrint=true;
    }
 

  



}