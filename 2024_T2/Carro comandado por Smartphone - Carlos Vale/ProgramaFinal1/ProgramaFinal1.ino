#include <SoftwareSerial.h>

const int ena = 10, in1 = 9, in2 = 8;  // Motor A
const int enb = 5, in3 = 7, in4 = 6;   // Motor B
char opcao;

SoftwareSerial bluetooth(2,3); //RX, TX

//sensor
const int sensorInfraB = A0;     // the number of the sensor pin
const int sensorInfraF = A1;     // the number of the sensor pin


int sensorStateB = 0;         // variable for reading the sensor status
int sensorStateF = 0;         // variable for reading the sensor status

void setup() {

  pinMode(ena, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);

  pinMode(enb, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
     
  pinMode(sensorInfraF, INPUT);
  pinMode(sensorInfraB, INPUT);

  randomSeed(analogRead(A5));

  Serial.begin(9600);
  bluetooth.begin(9600);
}



void loop() 
{
    //char opcao = 'S'; // Começa parado
    if (bluetooth.available() > 0) 
    {
        opcao = bluetooth.read(); // Atualiza o comando
        Serial.print("Comando recebido: ");
        Serial.println(opcao);       
    }

    // Leitura do sensor
    sensorStateF = digitalRead(sensorInfraF);
    sensorStateB = digitalRead(sensorInfraB);
    /*Serial.print("Comando recebido F: ");
    Serial.print(sensorStateF);
    Serial.print("\tComando recebido B: ");
    Serial.println(sensorStateB);   */     
  

    switch (opcao) 
    {
        case 'F':
          if (sensorStateF == LOW || sensorStateB == LOW) 
          {
            moveStop(); // Para se detectar obstáculo
            moveB();
            delay(1000);
            moveStop();
            if(random(2)) moveR();
            else moveL();
            delay(500);
            moveStop();
          } 
          else 
          {
            moveF();    // Anda para frente
          }
        break;
        case 'B':
          /*if (sensorStateF == LOW || sensorStateB == LOW) 
          {
            moveStop(); // Para se detectar obstáculo
          } 
          else 
          {*/
            moveB();    // Anda para frente
          //}          
          break;
        case 'L':
          moveL();
          break;
        case 'R':
          moveR();
          break;
        case 'S':
          moveStop();
          break;
     }

}

  void moveB() 
  {
    analogWrite(ena, 255);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enb, 255);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }

  void moveF() 
  {
    analogWrite(ena, 255);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enb, 255);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }

  void moveL() 
  {
    analogWrite(ena, 255);
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    analogWrite(enb, 255);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }

  void moveR() 
  {
    analogWrite(ena, 255);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    analogWrite(enb, 255);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }

  void moveStop() 
  {
    analogWrite(ena, 0);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    analogWrite(enb, 0);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }



