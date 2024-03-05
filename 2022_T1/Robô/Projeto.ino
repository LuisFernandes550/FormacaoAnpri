#include <SoftwareSerial.h>
#include <Ultrasonic.h>
SoftwareSerial mySerial(2, 3); // RX, TX

#define TRIGGER_PIN  11
#define ECHO_PIN     12

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

const int enA = 10, enB = 5, in1 = 9, in2 = 8, in3 = 7, in4 = 6;
char opcao;
int modo = 13;
int modoControlo;
float distancia;
long tempo;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(modo, INPUT);
  Serial.begin(9600);
  mySerial.begin(9600);
}
void loop() {
  modoControlo = digitalRead(modo); //interruptor para selecionar o modo
  Serial.println(modoControlo);
  if (modoControlo == HIGH) {
    controloAutomato();
  }
  else {
    controloBluetooth();
  }
}

void controloAutomato() { //Funcao automatico quando selecionada, o robot desvia-se sozinho dos obstaculos

  tempo = ultrasonic.timing();
  distancia = ultrasonic.convert(tempo, Ultrasonic::CM);
  if ( distancia >= 15) {
    moveF();
  }
  else {
    moveStop();
    delay(500);
    moveB();
    delay(500);
    moveR();
    delay(500);
  }
}

void controloBluetooth() { //Funcao manual ativada, ele aguarda comandos enviados pelo telemovel para se movimentar
  if (mySerial.available() > 0) {
    opcao = mySerial.read();
    switch (opcao) {
      case 'F':
        moveF();
        Serial.println("Frente");
        break;
      case 'B':
        moveB();
        Serial.println("Marcha atrás");
        break;
      case 'L':
        moveL();
        Serial.println("Esquerda");
        break;
      case 'R':
        moveR();
        Serial.println("Direita");
        break;
      case 'S':
        moveStop();
        Serial.println("Parar");
        break;
    }
  }
}
//direção do motor
void moveF() {
  controlSpeed(255, 255);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}
void moveB() {
  controlSpeed(255, 255);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void moveL() {
  controlSpeed(255, 255);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void moveR() {
  controlSpeed(255, 255);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}
void moveStop() {
  controlSpeed(0, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
void controlSpeed(int velA, int velB) {
  analogWrite(enA, velA);
  analogWrite(enB, velB);
}