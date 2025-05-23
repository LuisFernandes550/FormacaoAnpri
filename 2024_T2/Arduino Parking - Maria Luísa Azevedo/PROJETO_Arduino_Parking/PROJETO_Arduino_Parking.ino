#include <Ultrasonic.h>

/*
Formação Programação em Arduino
PROJETO FINAL
Luísa Azevedo
 * ----------------------------------
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

#define TRIGGER_PIN  2
#define ECHO_PIN     4

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);
LiquidCrystal_I2C lcd(0x27, 16,2);

const int motorPin = 8, potPin = A0;
int pot, angulo;
int nlvagos = 5;  // nº de lugares de estacionamento disponíveis

Servo myServo;

void setup()  {
  lcd.init();
  lcd.backlight();

  myServo.attach(motorPin, 500, 2500);
  myServo.write(0);
  lcd.setCursor(0, 0);
  lcd.print("Lugares vagos: " + (String) nlvagos);
  lcd.setCursor(0, 1);
  lcd.print("Bemvindos!");

  Serial.begin(9600);
  }

void loop() {
  float distancia;
  long tempo= ultrasonic.timing();

  distancia = ultrasonic.convert(tempo, Ultrasonic::CM);
  Serial.print("Tempo: " + (String) tempo);
  Serial.println(" Distância: " + (String) distancia);

  if(distancia<10 && nlvagos>=1) {
    myServo.write(90);
    Serial.println("Abrir");

    nlvagos = nlvagos - 1;
    lcd.setCursor(0, 0);
    lcd.print("Lugares vagos: " + (String) nlvagos);
    lcd.setCursor(0, 1);
    lcd.print("Seja Bem-vindo!");

    delay(5000);
  } else {
    if(nlvagos<1){
      myServo.write(0);
      Serial.println("Fechar");
      lcd.setCursor(0, 0);
      lcd.print("Lugares vagos: " + (String) nlvagos);
      lcd.setCursor(0, 1);
      lcd.print("PARQUE LOTADO! ");
      delay(500);
    } else {
       myServo.write(0);
       Serial.println("Fechar");
       
       delay(500);

    }
   
  }
}

