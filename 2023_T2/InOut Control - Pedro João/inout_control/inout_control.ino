#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <Ultrasonic.h>

#define ledPinR 13     //LED vermelho
#define ledPinG 12     //LED verde
#define btnPin 2       //botão
#define buzzerPin 11   //buzzer
#define servoPin 10    //motor servo
#define trigger1Pin 5  //emissor do sensor ultrassónico 1
#define echo1Pin 4     //recetor do sensor ultrassónico 1
#define trigger2Pin 7  //emissor do sensor ultrassónico 2
#define echo2Pin 6     //recetor do sensor ultrassónico 2

LiquidCrystal_I2C meuLCD(0x27, 16, 2);  //set the LCD address to 0x27 for a 16 chars and 2 line display
Servo meuServo;                         //motor Servo
Ultrasonic meuSensor1(trigger1Pin, echo1Pin);
Ultrasonic meuSensor2(trigger2Pin, echo2Pin);


int botao, in = 0, out = 0;    //variavel que armazena o estado do botão
float distancia1, distancia2;  //distancia do sensor ao objeto
int distDetecao = 60;
long tempo1, tempo2;  //tempo que o sinal demora a chegar desde que foi emitido
bool entrou = false, saiu = false;
int estadoE = 0, estadoS = 0;  //para guardar estados dos sensores. Caso a pessoa recue, não entra/sai
bool aEntrar = false, aSair = false;

//---------------------------------------------------------------------------------------------------------

void setup() {
  pinMode(ledPinR, OUTPUT);  // Definir a porta digital do led como OUTPUT
  pinMode(ledPinG, OUTPUT);  // Definir a porta digital do led como OUTPUT
  pinMode(btnPin, INPUT);    // Definir a porta digital do botão como INPUT
  meuLCD.init();             // inicializa o lcd
  meuLCD.backlight();        // ligar luz de fundo
  pinMode(buzzerPin, OUTPUT);
  meuServo.attach(servoPin);  //associar o pino 10 do motor ao objeto Servo + attach função associada aos servos
  meuServo.write(45);         // coloca seta na vertical
  Serial.begin(9600);
}

void loop() {

  atualizaLCD();
  botaoPressionado();
  mostraDistanciaEEstados();

  // se alguém em frente aos sensores » buzzer
  if (distancia1 < distDetecao || distancia2 < distDetecao) {
    tone(buzzerPin, 10000, 20);
  } else {
    noTone(buzzerPin);  //desliga buzzer
  }

  // se ENTRA, 3 segundos com led e motor
  if (confirmaEntrada()) {
    digitalWrite(ledPinG, HIGH);  // led verde
    meuServo.write(90);           // motor lado verde
    in++;
    atualizaLCD();
    delay(3000);
    digitalWrite(ledPinG, LOW);  // led verde
    meuServo.write(45);          // motor para meio
    estadoE = 0;
    entrou = false;
    aEntrar = false;
  }

  // se SAI, 3 segundos
  if (confirmaSaida()) {
    digitalWrite(ledPinR, HIGH);  // led vermelho
    meuServo.write(0);            // motor lado vermelho
    out++;
    atualizaLCD();
    delay(3000);
    digitalWrite(ledPinR, LOW);  // led vermelho
    meuServo.write(45);          // motor para meio
    estadoS = 0;
    saiu = false;
    aSair = false;
  }
}

//---------------------------------------------------------------------------------------------------------

void botaoPressionado() {
  botao = digitalRead(btnPin);  //Ler o estado do botão
  if (botao == 1) {
    digitalWrite(ledPinR, LOW);  //Desliga o led vermelho
    digitalWrite(ledPinG, LOW);  //Desliga o led verde
    meuServo.write(45);          // motor para meio
    in = 0;
    out = 0;
    entrou = false;
    saiu = false;
    estadoE = 0;
    estadoS = 0;
    noTone(buzzerPin);  //desliga buzzer
    atualizaLCD();
  }
}

void atualizaLCD() {
  meuLCD.setCursor(0, 0);  //(coluna, linha)
  meuLCD.print("IN: ");
  meuLCD.setCursor(4, 0);  //(coluna, linha)
  meuLCD.print(in);
  meuLCD.setCursor(6, 0);  //(coluna, linha)
  meuLCD.print(" | OUT: ");
  meuLCD.setCursor(14, 0);  //(coluna, linha)
  meuLCD.print(out);
  meuLCD.setCursor(3, 1);  //(coluna, linha)
  meuLCD.print("DENTRO: ");
  meuLCD.setCursor(11, 1);  //(coluna, linha)
  meuLCD.print(in - out);
}

void mostraDistanciaEEstados() {
  tempo1 = meuSensor1.timing();                             //devolve tempo (em microsegundos) entre envio e receção
  tempo2 = meuSensor2.timing();                             //devolve tempo (em microsegundos) entre envio e receção
  distancia1 = meuSensor1.convert(tempo1, Ultrasonic::CM);  //converte tempo em distância
  distancia2 = meuSensor2.convert(tempo2, Ultrasonic::CM);  //converte tempo em distância
  Serial.print("Distância 1: ");
  Serial.print(distancia1);
  Serial.print(" | Distância 2: ");
  Serial.print(distancia2);
  Serial.print(" | Estado Entrada: ");
  Serial.print(estadoE);
  Serial.print(" | Estado Saida: ");
  Serial.println(estadoS);
  delay(300);
}

boolean confirmaEntrada() {
  switch (estadoE) {
    case 0:
      if (distancia1 < distDetecao && aSair == false) {
        estadoE++;
        aEntrar = true;
      }  // 1 0 » estadoE=1
      break;
    case 1:
      if (distancia1 >= distDetecao && aEntrar == true) {
        estadoE--;
        aEntrar = false;
      }                                                                // 0 0 » estadoE=0
      if (distancia2 < distDetecao && aEntrar == true) { estadoE++; }  // 1 1 » estadoE=2
      break;
    case 2:
      if (distancia1 >= distDetecao && aEntrar == true) { estadoE++; }  // 0 1 » estadoE=3
      if (distancia2 >= distDetecao && aEntrar == true) { estadoE--; }  // 1 0 » estadoE=1
      break;
    case 3:
      if (distancia1 < distDetecao && aEntrar == true) { estadoE--; }  // 1 1 » estadoE=2
      if (distancia2 >= distDetecao && aEntrar == true) {
        estadoE++;
        entrou = true;
      }  // 0 0 » estadoE=4
      break;
  }
  return entrou;
}

boolean confirmaSaida() {
  switch (estadoS) {
    case 0:
      if (distancia2 < distDetecao && aEntrar == false) {
        estadoS++;
        aSair = true;
      }  // 0 1 » estadoS=1
      break;
    case 1:
      if (distancia1 < distDetecao && aSair == true) { estadoS++; }  // 1 1 » estadoS=2
      if (distancia2 >= distDetecao && aSair == true) {
        estadoS--;
        aSair = false;
      }  // 0 0 » estadoS=0
      break;
    case 2:
      if (distancia1 >= distDetecao && aSair == true) { estadoS--; }  // 0 1 » estadoS=1
      if (distancia2 >= distDetecao && aSair == true) { estadoS++; }  // 1 0 » estadoS=3
      break;
    case 3:
      if (distancia2 < distDetecao && aSair == true) { estadoS--; }  // 1 1 » estadoS=2
      if (distancia1 >= distDetecao && aSair == true) {
        estadoS++;
        saiu = true;
      }  // 0 0 » estadoS=4
      break;
  }
  return saiu;
}