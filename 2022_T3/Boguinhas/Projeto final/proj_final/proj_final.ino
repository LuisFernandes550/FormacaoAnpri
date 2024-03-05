/*O Circuito:
  Buzzer + pin 43
  DHT11 out 42
  LDR pin A15
  Bluetooth (10, 11) RX,TX no Mega
  OLED 0.96 SDA 20 SCL 21
  Ultrasónico1 trigger 49 echo 51
  Ultrasónico2 trigger 49 echo 51
  LedFdir pin 53 ledFesq pin 51, ledTdir pin 46, ledTesq pin 47
  ledpiscaFesq pin 52, ledpiscaFdir pin 49, ledpiscaTesq pin 50, ledpiscaTdir pin 48

  Posições dos ícones no OLED 0.96 128x64
  display.drawBitmap(21, 3, temp, 16, 16, 1);
  display.drawBitmap(21, 3, gelo, 16, 16, 1);
  display.drawBitmap(38, 3, BT, 16, 16, 1);
  display.drawBitmap(38, 3, NBT, 16, 16, 1);
  display.drawBitmap(0, 3, sol, 16, 16, 1);
  display.drawBitmap(0, 3, lua, 16, 16, 1);
  display.drawBitmap(110, 18, piscadir, 16, 16, 1);
  display.drawBitmap(0, 18, piscaesq, 16, 16, 1);
  display.drawBitmap(87, 3, piscas, 16, 16, 1);
  display.drawBitmap(56, 3, frente, 16, 16, 1);
  display.drawBitmap(40, 14, esq, 16, 16, 1);
  display.drawBitmap(72, 14, dir, 16, 16, 1);
  display.drawBitmap(57, 28, tras, 16, 16, 1);
*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>  //Biblioteca OLED i2c
#include <Adafruit_GFX.h>      //Biblioteca OLED i2c

#include <SoftwareSerial.h>
SoftwareSerial bluetooth(10, 11);  //RX, TX no Mega

#include "logos.h"       //Logotipo e ícones
#include <Ultrasonic.h>  //Biblioteca do sensor Ultrasónico HC-SR04
#include "DHT.h"         //Biblioteca sensor DHT11


#include <Servo.h>
Servo myservo;  // Cria servo objeto para controlar um servo
int pos = 0;    // Variável para guardar a posição do servo

#include <AFMotor.h>

#define FORWARD 2   //VALOR DA BIBLIOTECA 1
#define BACKWARD 1  //VALOR DA BIBLIOTECA 2

AF_DCMotor motor(1);  //motor DC com fan

AF_DCMotor motor1(2);
AF_DCMotor motor2(3);


//Definição de pins para o sensor Ultrasónico frontal
#define triggerPinF 39
#define echoPinF 41

//Definição de pins para o sensor Ultrasónico traseiro
#define triggerPinT 35
#define echoPinT 33

//Definição de pins para o sensor DHT
#define DHTPIN 42

//Definição dos pins dos leds (luzes no carro)
//ledFdir pin 53 ledFesq pin 51, ledTdir pin 46, ledTesq pin 47, ledpiscaFesq pin 52, ledpiscaFdir pin 49, ledpiscaTesq pin 50, ledpiscaTdir pin 48
const int ledFdir = 53, ledFesq = 51, ledTdir = 46, ledTesq = 47, ledpiscaFesq = 52, ledpiscaFdir = 49, ledpiscaTesq = 50, ledpiscaTdir = 48;

//Definição do pin sensor LDR porta A15
const int ldrPin = A15;

Ultrasonic ultrasonic1(triggerPinF, echoPinF);  //Criar um objeto do tipo Ultrasonic frontal
Ultrasonic ultrasonic2(triggerPinT, echoPinT);  //Criar um objeto do tipo Ultrasonic frontal

#define DHTTYPE DHT11

#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_ADDR 0x3C

// Altura e largura logo (alterar tamanho se necessário)
#define LOGO_HEIGHT 64
#define LOGO_WIDTH 128

Adafruit_SSD1306 display(OLED_WIDTH, OLED_HEIGHT);

int ldr;            //armazena  o valor de luminosidade do sensor
float temperatura;  //armazena o valor da temperatura
float humidade;     //armazena o valor da humidade

float distanciaT;  //distância do sensor ultrasónico em relação ao objeto traseiro
float distanciaF;  //distância do sensor ultrasónico em relação ao objeto frontal
float barraF;      //valor para visualização da distância frontal no OLED
float barraT;      //valor para visualização da distância Traseira no OLED
//tempo que o sinal
long tempoT;
long tempoF;

DHT dht(DHTPIN, DHTTYPE);

char letra;  //entrada na app

int estadoluzes, estadopiscas;

const int buzzer = 43;

void setup() {
  bluetooth.begin(9600);  //inicializa BT

  // VELOCIDADE DOS MOTORES
  motor1.setSpeed(180);  // 255 VELOCIDADE MAXIMA
  motor2.setSpeed(180);  // 255 VELOCIDADE MAXIMA

  // MOTORES INICIAM PARADOS
  motor1.run(RELEASE);
  motor2.run(RELEASE);

  estadoluzes = 0;
  estadopiscas = 0;

  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  // Limpa display
  display.clearDisplay();

  // Preenche tela com bitmap
  display.drawBitmap(
    (display.width() - LOGO_WIDTH) / 2,
    (display.height() - LOGO_HEIGHT) / 2,
    LOGO, LOGO_WIDTH, LOGO_HEIGHT, 1);
  // Exibe no display
  display.display();
  // Aguarda 1 s
  delay(1000);
  // Inverte cores no display
  display.invertDisplay(true);
  // Aguarda 1 s
  delay(1000);
  // Volta às cores originais no display
  display.invertDisplay(false);
  // Aguarda 1 s

  // Limpa display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("ANPRI");
  display.println("Programacao");
  display.println("com Arduinos");
  display.println("");
  display.println("Projeto final");
  display.println("Raquel Mesquita");

  display.display();
  delay(1000);

  pinMode(ledFesq, OUTPUT);
  pinMode(ledFdir, OUTPUT);
  pinMode(ledTesq, OUTPUT);
  pinMode(ledTdir, OUTPUT);
  pinMode(ledpiscaFesq, OUTPUT);
  pinMode(ledpiscaFdir, OUTPUT);
  pinMode(ledpiscaTesq, OUTPUT);
  pinMode(ledpiscaTdir, OUTPUT);
  dht.begin();
  digitalWrite(ledFesq, 0);
  digitalWrite(ledFdir, 0);
  digitalWrite(ledTesq, 0);
  digitalWrite(ledTdir, 0);
  digitalWrite(ledpiscaFesq, 0);
  digitalWrite(ledpiscaFdir, 0);
  digitalWrite(ledpiscaTesq, 0);
  digitalWrite(ledpiscaTdir, 0);
  Serial.begin(9600);

  // Conecta o servo no pino 10 para o servo objeto
  myservo.attach(10);

  //Define a velocidade inicial do motor e imobiliza-o
  motor.setSpeed(200);
  motor.run(RELEASE);
  
}

//função controla o apagar das luzes dianteiras e traseiras
void apagaluzes() {
  digitalWrite(ledFesq, 0);
  digitalWrite(ledFdir, 0);
  digitalWrite(ledTesq, 0);
  digitalWrite(ledTdir, 0);
  estadoluzes = 0;
}

//função controla o acender das luzes dianteiras e traseiras
void acendeluzes() {
  digitalWrite(ledFesq, 1);
  digitalWrite(ledFdir, 1);
  digitalWrite(ledTesq, 1);
  digitalWrite(ledTdir, 1);
  estadoluzes = 1;
}

//função liga 4 piscas
void acende4piscas() {
  //if (estadopiscas = 0) {
  display.drawBitmap(87, 3, piscas, 16, 16, 1);
  digitalWrite(ledpiscaFesq, 1);
  digitalWrite(ledpiscaFdir, 1);
  digitalWrite(ledpiscaTesq, 1);
  digitalWrite(ledpiscaTdir, 1);
  estadopiscas = 1;
  //}
}
//função desliga 4 piscas
void desligapiscas() {
  if (estadopiscas = 1) {
    digitalWrite(ledpiscaFesq, 0);
    digitalWrite(ledpiscaFdir, 0);
    digitalWrite(ledpiscaTesq, 0);
    digitalWrite(ledpiscaTdir, 0);
    estadopiscas = 0;
  }
}

void verifgelo() {
  //Função controla o ícone de aviso de gelo na estrada
  if (temperatura < 28,8) {  //temp abaixo dos 4º surge símbolo de gelo na estrada
    //escreve simbolo gelo
    // display.drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
    display.drawBitmap(21, 3, gelo, 16, 16, 1);
  } else {
    //escreve simbolo temp
    display.drawBitmap(21, 3, temp, 16, 16, 1);
  }
}

void moverFrente() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void moverTras() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
}
void moverDireita() {
  motor2.run(FORWARD);
  motor1.run(RELEASE);
}

void moverEsquerda() {
  motor2.run(RELEASE);
  motor1.run(FORWARD);
}

void para() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}

void loop() {
  uint8_t i;

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //Lê valores de temperatura e humidade do sensor DHT11
  temperatura = dht.readTemperature();
  humidade = dht.readHumidity();

  ldr = analogRead(ldrPin);  //Lê valor de luminosidade do sensor LDR

  tempoT = ultrasonic2.timing();
  distanciaT = ultrasonic2.convert(tempoT, Ultrasonic::CM);

  tempoF = ultrasonic1.timing();
  distanciaF = ultrasonic1.convert(tempoF, Ultrasonic::CM);

//verifica distância frontal
  if (distanciaF > 10 || distanciaF < 20) {
    //calcula x a preencher no ecrã
    barraF = (20 - distanciaF) * 12, 8;
    for (int x = 0; x < barraF; x++)
      for (int y = 0; y < 2; y++) {
        // Desenha linha vertical
        display.drawPixel(x, y, SSD1306_WHITE);
      }
  }
  
  //verifica distância traseira
  if (distanciaT > 10 || distanciaT < 20) {
    //calcula x a preencher no ecrã
    barraT = (20 - distanciaT) * 12, 8;
    for (int x = 0; x < barraT; x++)
      for (int y = 62; y < 64; y++) {
        // Desenha linha vertical
        display.drawPixel(x, y, SSD1306_WHITE);
      }
  } else{
    if (distanciaT < 10) {
      tone(buzzer, 164);
      display.drawBitmap(56, 14, parado, 16, 16, 1);
      display.display();
      desligapiscas();
      apagaluzes();
      //para o carro
      para();
      noTone(buzzer);   
    }
  }

  //Verifica luminosidade, liga luzes e mostra ícone de noite ou mostra apenas ícone de dia
  if (ldr < 20) {
    //ícone noite (lua)
    display.drawBitmap(0, 3, lua, 16, 16, 1);
    display.display();
    acendeluzes();
  } else {
    //ícone dia (sol)
    display.drawBitmap(0, 3, sol, 16, 16, 1);
    display.display();
    apagaluzes();
  }

  verifgelo();

  display.setCursor(0, 53);
  display.print("T:");

  display.setCursor(13, 53);
  display.print(temperatura);
  display.print((char)223);  //escreve o símbolo de grau
  display.println("C");

  display.setCursor(65, 53);
  display.println("H:");

  display.setCursor(80, 53);
  display.print(humidade);
  display.print("%");

  //Liga ventoinha se temp maior que 30º
  if (temperatura > 30.00) {  //temp acima dos 30 liga a ventoinha para ventilação até diminuição da temp
    //escreve simbolo fan
    display.drawBitmap(110, 3, fan, 16, 16, 1);
    display.display();
    // Liga o motor
    motor.run(FORWARD);
    // Acelera de zero à velocidade máxima
    for (i = 0; i < 255; i++) {
      motor.setSpeed(i);
      delay(10);
    }
  } else {
    display.drawBitmap(110, 3, fanoff, 16, 16, 1);
    display.display();
    // Desacelera da velocidade máxima para zero
    for (i = 255; i != 0; i--) {
      motor.setSpeed(i);
      delay(10);
    }
    // Desliga o motor
    motor.run(RELEASE);
  }
  display.display();
  //delay(100);

  /*if (bluetooth.available()) {  // Checks whether data is comming from the serial port
    letra = bluetooth.read();       // Reads the data from the serial port*/
  if (Serial.available()>0){
    letra = Serial.read();
  }
  //Se comunicação serial está disponível
  //display.drawBitmap(110, 3, fanoff, 16, 16, 1);
  //display.display();
  Serial.print("comando ");
  Serial.println(letra);
  switch (letra) {
    case 'F':  //foward
      display.drawBitmap(56, 0, frente, 16, 16, 1);
      display.display();
      //mover frente
      moverFrente();
      desligapiscas();
      //delay(100);
      break;
    case 'B':  //back
      //mover trás
      moverTras();
      desligapiscas();
      //delay(100);
      break;
    case 'L':  //left
      display.drawBitmap(40, 14, esqu, 16, 16, 1);
      display.drawBitmap(0, 18, piscaesq, 16, 16, 1);
      display.display();
      digitalWrite(ledpiscaFesq, 1);
      digitalWrite(ledpiscaTesq, 1);
      digitalWrite(ledpiscaFdir, 0);
      digitalWrite(ledpiscaFdir, 0);
      //mover esquerda
      moverEsquerda();
      Serial.println("Moveu para esquerda");
      //delay(100);
      break;
    case 'R':  //right
      display.drawBitmap(72, 14, dire, 16, 16, 1);
      display.drawBitmap(110, 18, piscadir, 16, 16, 1);
      display.display();
      digitalWrite(ledpiscaFesq, 0);
      digitalWrite(ledpiscaTesq, 0);
      digitalWrite(ledpiscaFdir, 1);
      digitalWrite(ledpiscaTdir, 1);
      //mover direita
      moverDireita();
      Serial.println("Moveu para direita");
      break;
      //delay(100);
    case 'U':  //liga 4 piscas
      //Marcha de emergência
      acende4piscas();
      //delay(100);
      break;
    case 'u':  //desliga 4 piscas
      desligapiscas();
      apagaluzes();
      break;
    case 'S':  //stop
      display.drawBitmap(56, 14, parado, 16, 16, 1);
      display.display();
      desligapiscas();
      apagaluzes();
      //para o carro
      para();
      //delay(100);
      break;
    case 'W':  //acende luzes
      acendeluzes();
      //delay(100);
      break;
    case 'w':  //desliga luzes
      apagaluzes();
      //delay(100);
      break;
    default:
      display.drawBitmap(56, 14, parado, 16, 16, 1);
      display.display();
      desligapiscas();
      apagaluzes();
      //para o carro
      para();
      //delay(100);
      break;
  }
  display.display();
}