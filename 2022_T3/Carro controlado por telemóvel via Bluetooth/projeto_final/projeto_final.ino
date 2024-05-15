#include <SoftwareSerial.h>


int enA = 10;
int in1 = 9;
int in2 = 8;
int enB = 5;
int in3 = 7;
int in4 = 6;
int LEDF = 11;
int LEDT = 12;

SoftwareSerial mySerial(0,1);
char opcao;

void setup() {
  Serial.begin(9600);
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
pinMode(LEDF, OUTPUT);
  pinMode(LEDT, OUTPUT);
  
  mySerial.begin(9600);
}

void controlSpeed(int velA, int velB){
  analogWrite(enA,velA);
    analogWrite(enB,velB);
}
void LIGAF(){
digitalWrite(LEDF, HIGH);
    }
void DESLIGAF(){
digitalWrite(LEDF, LOW);
    }
void LIGAT(){
digitalWrite(LEDT, HIGH);
    }
void DESLIGAT(){
digitalWrite(LEDT, LOW);
    }
void moveF(){
controlSpeed(255,255);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
    }
void moveB(){
  controlSpeed(255,255);
  digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }
void moveL(){
  controlSpeed(255,255);
  digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
    }
void moveR(){
  controlSpeed(255,255);
   digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
    }

    void moveStop(){
  controlSpeed(0,0);
   digitalWrite(12, LOW);
      digitalWrite(11, LOW);
      digitalWrite(10, LOW);
      digitalWrite(9, LOW);
    }
void loop() {

  opcao = mySerial.read();
   Serial.println(opcao);
  if (Serial.available() > 0) {
    opcao = mySerial.read();
    
    switch (opcao){
   case  'F':
      moveF();
       LIGAF();
       DESLIGAT();
      Serial.println("Frente");
      break;
    case  'B':
      moveB();
      DESLIGAF();
      LIGAT();
      Serial.println("Marcha atrás");
      break;
    case  'L':
      moveL();
      Serial.println("Esquerda");
      break;
 case  'R':
      moveR();
      Serial.println("Direita");
      break;
 case  'W':
      LIGAF();
      Serial.println("Liga LuZ F");
      break;
      case  'w':
      DESLIGAF();
      Serial.println("DesLiga LuZ F");
      break;
case  'U':
      LIGAT();
      Serial.println("Liga LuZ T");
      break;
      case  'u':
      DESLIGAT();
      Serial.println("DesLiga LuZ T");
      break;
      
    case  'S':
      moveStop();
      Serial.println("Parar");
      break;

}
  }
}
