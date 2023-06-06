#include <Encoder.h>

Encoder myEnc(2, 5);
Encoder MyEnc2(4, 6);

// var asservissement -- debut
long commande1 = 0;
long commande2 = 0;
int erreur1;
int erreur2;

float kp = 50;
float ki = 0.02;

long t_history = 0;
long t = 0;

float accumulation1 = 0;
float accumulation2 = 0;


// var asservissement -- fin

int pin1 = A4; // bleu              bleu | jaune  
int pin2 = A5; // jaune             -----------------------      
int pin3 = A2; // rose              rose | violet
int pin4 = A3; // violet

int pot = A15; // potentiometre

String space = " - ";
int sensorVal_1, sensorVal_2, sensorVal_3, sensorVal_4; // resistance de 1OO ohm

void setup() {
  
  Serial.begin(9600);  

  pinMode(3, OUTPUT);  
  pinMode(11, OUTPUT);  
  pinMode(12, OUTPUT);  
  pinMode(13, OUTPUT);  
 
  pinMode(pot,INPUT);

  Serial.begin(9600);
  pinMode(sensorVal_1, INPUT);
  pinMode(sensorVal_2, INPUT);
  pinMode(sensorVal_3, INPUT);
  pinMode(sensorVal_4, INPUT);

  

}

void loop() {

  int valPotentio = analogRead(pot);
  valPotentio = map(valPotentio, 0, 1023, 0, 1080); // angle 180 à redéfinir 
  Serial.println(valPotentio);
  // voir quel moteur est contrôlé par le potentiometre
  // il faut aussi définir l'angle limite pour les 2 moteurs
  // L'angle limite facilement définissable avec le potentiometre
  // pour l'autre moteur 

  while(valPotentio<90){ // faire des tests et voir quels sont les valeurs/le domaine où le potentiometre pourra marcher ex: entre 20 et 200
    
    sensorVal_1 = analogRead(pin1); // photosensor 1
    sensorVal_2 = analogRead(pin2); // photosensor 2
    sensorVal_3 = analogRead(pin3); // photosensor 3
    sensorVal_4 = analogRead(pin4); // photosensor 4
    Serial.print(sensorVal_1+space);
    Serial.print(sensorVal_2+space);
    Serial.print(sensorVal_3+space);
    Serial.print(sensorVal_4);
    Serial.println();

    t = millis();
    // erreur : diff1, etc
    //accumulation = accumulation + ki*erreur*(t - t_history);
    //commande = kp * erreur + accumulation - kd*pente;
  
    // gestion rotation moteur en fonction des capteurs
  
    int diff1 = sensorVal_1 - sensorVal_2;
    int diff2 = sensorVal_3 - sensorVal_4;
    int ecart = 20;
  
    if(abs(diff1)>ecart || abs(diff2)>ecart){
      //Serial.println("HAUT");
      erreur1 = diff1 + diff2;
      accumulation1 = accumulation1 + ki*erreur1*(t - t_history);
      if(abs(accumulation1)>100){ // on fixe un seuil à l'accumulation!
        accumulation1=100;  
        }
      commande1 = kp * erreur1 + accumulation1;
      motor1((-1)*commande1);
      Serial.print(t);
      Serial.print(" ; ");
      Serial.print(erreur1);
      Serial.print(" ; ");
      Serial.print(accumulation1);
      Serial.print(" ; ");
      Serial.print(commande1);
      Serial.println(" ; ");
    } 
    else{
      motor1(0);
      }
    int diff3 = sensorVal_1 - sensorVal_3;
    int diff4 = sensorVal_2 - sensorVal_4;
  
    if(abs(diff3)>ecart || abs(diff4)>ecart){
      erreur2 = diff3 + diff4;
      accumulation2 = accumulation2 + ki*erreur2*(t - t_history);
      if(abs(accumulation2)>100){
        accumulation2=100;  
        }
      commande2 = kp * erreur2 + accumulation2;
      motor2(commande2);
      Serial.print(t);
      Serial.print(" ; ");
      Serial.print(erreur2);
      Serial.print(" ; ");
      Serial.print(accumulation2);
      Serial.print(" ; ");
      Serial.print(commande2);
      Serial.println(" ; ");
    } 
    else{
      motor2(0);
      }
      
    t_history = t;

    
      }

  // on cherche à arrêter le moteur pour pas qu'il "tombe" et qu'il se stop à son angle limite
  motor2(0); // dépend du moteur où on va fixer le potentiometre
  
}

void motor1(int vitesse) { // vitesse de -255 à 255  

   if (vitesse > 0) {    

      digitalWrite(12, HIGH);  

   } else {    

      digitalWrite(12, LOW);  

   }  

   vitesse = abs(vitesse);  

   if (vitesse > 255) vitesse = 255;  

   analogWrite(3, vitesse);

}
void motor2(int vitesse) { // vitesse de -255 à 255  

   if (vitesse > 0) {    

      digitalWrite(13, HIGH);  

   } else {    

      digitalWrite(13, LOW);  

   }  

   vitesse = abs(vitesse);  

   if (vitesse > 255) vitesse = 255;  

   analogWrite(11, vitesse);

}
