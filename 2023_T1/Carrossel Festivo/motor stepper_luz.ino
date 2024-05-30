#include <Stepper.h>

#define ledPinR 6
#define ledPinG 5
#define ledPinB 3

#define btnPin 2

const int stepsPerRevolution = 2048;
int botao = 1;
int flag = 1;

Stepper myStepper = Stepper(stepsPerRevolution, 8, 10, 9, 11);

void setup() {

  pinMode(ledPinR, OUTPUT);
  pinMode(ledPinG, OUTPUT);
  pinMode(ledPinB, OUTPUT);

  myStepper.setSpeed(15);
  pinMode(btnPin, INPUT);
}

void loop() {
  //cogigo para luz
  randomSeed(analogRead(A0));
  analogWrite(ledPinR, random(0, 256));
  analogWrite(ledPinG, random(0, 256));
  analogWrite(ledPinB, random(0, 256));


  //codigo para motor
  if ((botao == 1 && flag == 1) || (botao == 0 && flag == 1)) {

    myStepper.step(-stepsPerRevolution);
    flag = 1;
    randomSeed(analogRead(A0));
    analogWrite(ledPinR, random(0, 256));
    analogWrite(ledPinG, random(0, 256));
    analogWrite(ledPinB, random(0, 256));
  }

  else {
    myStepper.step(stepsPerRevolution);
    flag = 0;
    randomSeed(analogRead(A0));
    analogWrite(ledPinR, random(0, 256));
    analogWrite(ledPinG, random(0, 256));
    analogWrite(ledPinB, random(0, 256));
  }

  botao = digitalRead(btnPin);
  if (botao == 1) {
    if (flag == 0) {
      flag = 1;
    } else flag = 0;
  }
}
