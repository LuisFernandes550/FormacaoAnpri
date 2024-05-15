// Circuito Braga Parque
// Luís Santos 2023.06.18; 2023.07.20
// Sensor ultra-sons modificado
#include <Ultrasonic.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//#include "DHT.h"
#define TRIGGER_PIN1 11
#define TRIGGER_PIN2 13
#define ECHO_PIN1 12
#define ECHO_PIN2 10
#define red1Pin 6    //LED red sensor 1
#define green1Pin 5  //LED green sensor 1
#define red2Pin 4    //LED red sensor 1
#define green2Pin 3  //LED green sensor 1

Ultrasonic ultrasonic1(TRIGGER_PIN1, ECHO_PIN1);
Ultrasonic ultrasonic2(TRIGGER_PIN2, ECHO_PIN2);
LiquidCrystal_I2C lcd(0x27, 16, 4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

int lug1 = 0;  // Guarda estado do sensor 1 (0: sem carro; 1:  com carro)
int lug2 = 0;  // Guarda estado do sensor 2 (0: sem carro; 1:  com carro)
int total = 100;

void setup() {
  // put your setup code here, to run once:
  pinMode(red1Pin, OUTPUT);
  pinMode(red2Pin, OUTPUT);
  pinMode(green1Pin, OUTPUT);
  pinMode(green2Pin, OUTPUT);
  digitalWrite(red1Pin, HIGH);
  digitalWrite(red2Pin, HIGH);
  digitalWrite(green1Pin, LOW);
  digitalWrite(green2Pin, LOW);

  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
}

void loop() {
  // put your main code here, to run repeatedly:
  float distancia1, distancia2;
  long tempo1 = ultrasonic1.timing();
  long tempo2 = ultrasonic2.timing();

  distancia1 = ultrasonic1.convert(tempo1, Ultrasonic::CM);
  distancia2 = ultrasonic2.convert(tempo2, Ultrasonic::CM);

  // Monitor série permanece ligado a tempos e distancias para controlo
  Serial.print("T1: ");
  Serial.print(tempo1);
  Serial.print("ms   Dist1: ");
  Serial.print(distancia1, 2);  //2o parametro: no. decimais
  Serial.print("cm    ");

  Serial.print("T2: ");
  Serial.print(tempo2);
  Serial.print("ms   Dist2: ");
  Serial.print(distancia2, 2);  //2o parametro: no. decimais
  Serial.println("cm");

  // Atualização do estado dos lugares
  if (distancia1 < 10) {
    lug1 = 1;
    digitalWrite(red1Pin, LOW);
    digitalWrite(green1Pin, HIGH);
  }
  if (distancia2 < 10) {
    lug2 = 1;
    digitalWrite(red2Pin, LOW);
    digitalWrite(green2Pin, HIGH);
  }
  if (distancia1 >= 10) {
    lug1 = 0;
    digitalWrite(red1Pin, HIGH);
    digitalWrite(green1Pin, LOW);
  }
  if (distancia2 >= 10) {
    lug2 = 0;
    digitalWrite(red2Pin, HIGH);
    digitalWrite(green2Pin, LOW);
  }

  // Display, `a entrada do Parque, exibe o numero de lugares dispoiveis
  lcd.setCursor(0, 0);
  lcd.print("BRAGA PARQUE");
  lcd.setCursor(0, 1);
  lcd.print("Total: 100 lugares");
  lcd.setCursor(0, 2);
  lcd.print("Disponiveis: ");
  lcd.print(2 - lug1 - lug2);

  delay(1000);
}