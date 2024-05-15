#include "MFRC522.h"

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522(SS_PIN, RST_PIN);

String tagID = "";
int check=0;
String Tag = "E32FFE2E";

#define gLed 2 
#define yLed 3 
#define rLed 4 

#define servo 7

void setup() {
Serial.begin(9600);
SPI.begin(); 
mfrc522.PCD_Init(); 

pinMode(gLed, OUTPUT);
pinMode(yLed, OUTPUT);
pinMode(rLed, OUTPUT);

pinMode(servo, OUTPUT);
servoPulse(servo, 30); 

digitalWrite(gLed, 0);
digitalWrite(yLed, 0);
digitalWrite(rLed, 1);

delay(500);
}

void loop() {
  while(getID()){    
    Serial.println(tagID);
    check = 0;  
    
    if(tagID == Tag){
      check=1;
      digitalWrite(gLed, 1);
      digitalWrite(yLed, 0);
      digitalWrite(rLed, 0);
      for(int angle=30; angle<=140; angle++){
        servoPulse(servo, angle);
        }
      delay(5000);
      digitalWrite(gLed, 0);
      digitalWrite(yLed, 1);
      digitalWrite(rLed, 0);
      delay(1000);
      for(int angle=140; angle>=30; angle--){
        servoPulse(servo, angle);
        }
     
     }
    
  
if(check==0){
  for(int x=0; x<10; x++){
    digitalWrite(gLed, 0);
    digitalWrite(yLed, 0);
    digitalWrite(rLed, 0);
    delay(500);  
    digitalWrite(yLed, 1);
    delay(500); 
  }  
}
      
digitalWrite(gLed, 0);
digitalWrite(yLed, 0);
digitalWrite(rLed, 1);
}
delay(10);
}


boolean getID(){
  if ( ! mfrc522.PICC_IsNewCardPresent()) { 
  return false;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) { 
  return false;
  }
  tagID = "";
  for ( uint8_t i = 0; i < 4; i++) { 
  
    tagID.concat(String(mfrc522.uid.uidByte[i], HEX)); 

  }
  tagID.toUpperCase();
  mfrc522.PICC_HaltA(); 
  return true;
}

void servoPulse (int pin, int angle){
  int pwm = (angle*11) + 500;      
  digitalWrite(pin, HIGH);
  delayMicroseconds(pwm);
  digitalWrite(pin, LOW);
  delay(30);                   
}
