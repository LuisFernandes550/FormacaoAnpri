#include <Wire.h>               //Esta biblioteca permite comunicar com dispositivos I2C
#include <LiquidCrystal_I2C.h>  //Esta biblioteca tem funções para os ecrãs LCD com interface I2C
#include <Servo.h>              //Esta biblioteca permite controlar o motor servo
#include <Ultrasonic.h>         //Esta biblioteca permite trabalhar com o sensor ultrassónico HC-SR04

#define servoPin 9  //Pino onde está ligado o motor servo
#define trigPin 13  //Pino trigger do sensor HC-SR04
#define echoPin 12  //Pino echo do sensor HC-SR04
#define btnPin 3    //Pino onde está ligado o botão

#define ledpin_verde 7     //porta digital led verde
#define ledpin_amarelo 6   //porta digital led amarelo
#define ledpin_vermelho 5  //porta digital led vermelho

#define buzzerPin 4  //Pino onde está ligado o buzzer


Servo myServo;                            //cria um objeto do tipo Servo
Ultrasonic ultrasonic(trigPin, echoPin);  // cria um objeto do tipo Ultrasonic

//Inicializa o display no endereço 0x27
LiquidCrystal_I2C lcd(0x27, 16, 2);

//distância do sensor em relação ao objeto
float distancia;

//tempo que o sinal demora a chegar desde que foi emitido
long tempo;

int pos = 0;  // variable to store the servo position
int aberto = 0;


void setup() {
  Serial.begin(9600);
  pinMode(ledpin_verde, OUTPUT);
  pinMode(ledpin_amarelo, OUTPUT);
  pinMode(ledpin_vermelho, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(btnPin, INPUT);
  pinMode(trigPin, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);   // Sets the echoPin as an Input
  lcd.init();                // inicializa o lcd
  lcd.backlight();           // acende a backlight
  myServo.attach(servoPin);  // attaches the servo on pin 9 to the servo object
  myServo.write(0);
  delay(200);
}

void loop() {
  lcd.clear();
  if (aberto == 0) {
    esperaBotao();
    if (digitalRead(btnPin)) {  //Se o botão estiver pressionado
      abrirCancela();
    }
  }
  if (aberto == 1) {
    lerDist();
    esperaPassar();
    if (distancia < 5) {
      fecharCancela();
    }
  }
}



void lerDist() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  tempo = ultrasonic.timing();
  distancia = ultrasonic.convert(tempo, Ultrasonic::CM);
}

void esperaBotao() {
  lcd.setCursor(0, 0);
  lcd.print("Para passar, ");
  lcd.setCursor(0, 1);
  lcd.print("Premir o bot");
  lcd.print((char)227);
  lcd.print("o");
  //Liga o LED vermelho e desliga os outros
  digitalWrite(ledpin_verde, LOW);
  digitalWrite(ledpin_amarelo, LOW);
  digitalWrite(ledpin_vermelho, HIGH);
  delay(100);  // Wait for 3000 millisecond(s)
}

void abrirCancela() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Abertura ");
  lcd.setCursor(0, 1);
  lcd.print("a decorrer ");
  for (pos = 0; pos <= 90; pos += 2) {  // goes from 0 degrees to 90 degrees
    // in steps of 1 degree
    tone(buzzerPin, 1000);
    delay(100);
    noTone(buzzerPin);
    myServo.write(pos);  // tell servo to go to position in variable 'pos'
    //Liga o LED amarelo e desliga os outros
    digitalWrite(ledpin_verde, LOW);
    digitalWrite(ledpin_amarelo, HIGH);
    delay(100);
    digitalWrite(ledpin_amarelo, LOW);
    digitalWrite(ledpin_vermelho, LOW);
    noTone(buzzerPin);  //até ser interrompida pela função noTone
    delay(100);         // waits 15ms for the servo to reach the position
  }
  aberto = 1;
}

void esperaPassar() {
  //Liga o LED verde e desliga os outros
  digitalWrite(ledpin_verde, HIGH);
  digitalWrite(ledpin_amarelo, LOW);
  digitalWrite(ledpin_vermelho, LOW);
  delay(100);  // Wait for 100 millisecond(s)
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cancela Aberta ");
  lcd.setCursor(0, 1);
  lcd.print("Pode passar ");
  tone(buzzerPin, 1000);
  delay(100);  // Wait for 100 millisecond(s)
}

void fecharCancela() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Fecho ");
  lcd.setCursor(0, 1);
  lcd.print("a decorrer ");
  for (pos = 90; pos > 0; pos -= 2) {  // goes from 0 degrees to 90 degrees
    // in steps of 1 degree
    tone(buzzerPin, 1000);
    delay(100);
    noTone(buzzerPin);
    myServo.write(pos);  // tell servo to go to position in variable 'pos'
    //Liga o LED amarelo e desliga os outros
    digitalWrite(ledpin_verde, LOW);
    digitalWrite(ledpin_amarelo, HIGH);
    delay(100);
    digitalWrite(ledpin_amarelo, LOW);
    digitalWrite(ledpin_vermelho, LOW);
    noTone(buzzerPin);  //até ser interrompida pela função noTone
    delay(15);          // waits 15ms for the servo to reach the position
  }
  aberto = 0;
}


