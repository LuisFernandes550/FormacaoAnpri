#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Definições do LCD I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definições do Servo
Servo cancela;
const int servoPin = 2;
const int aberto = 90;
const int fechado = 0;

// Definições dos sensores LDR
const int ldr1Pin = A0;
const int ldr2Pin = A1;
int ldr1Value = 0;
int ldr2Value = 0;
const int threshold = 90; // Valor de limiar para detectar ocupação

// Definições dos LEDs
const int ledVerde1 = 12;
const int ledVermelho1 = 13;
const int ledVerde2 = 8;
const int ledVermelho2 = 9;

// Definições do sensor ultrassônico
const int trigPin = 6;
const int echoPin = 4;
long duration;
int distance;
const int distanciaLimite = 10; // Distância limite para abertura da cancela

// Variáveis de tempo de ocupação
unsigned long tempoOcupacao1 = 0;
unsigned long tempoOcupacao2 = 0;
unsigned long inicioOcupacao1 = 0;
unsigned long inicioOcupacao2 = 0;
bool lugar1Ocupado = false;
bool lugar2Ocupado = false;


void setup() {
  // Inicialização do LCD
  lcd.init();
  lcd.backlight();
  
  // Inicialização do Servo
  cancela.attach(servoPin);
  cancela.write(fechado);

  // Configuração dos pinos dos LEDs
  pinMode(ledVerde1, OUTPUT);
  pinMode(ledVermelho1, OUTPUT);
  pinMode(ledVerde2, OUTPUT);
  pinMode(ledVermelho2, OUTPUT);

  // Configuração dos pinos do sensor ultrassônico
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Mensagem inicial no LCD
  lcd.setCursor(0, 0);
  lcd.print("    G. P. E.    ");
  lcd.setCursor(0, 1);
  lcd.print("Inicializando...");
  delay(2000);
  lcd.clear();
}

void loop() {

  // Leitura dos valores dos LDRs
  ldr1Value = analogRead(ldr1Pin);
  ldr2Value = analogRead(ldr2Pin);

  // Monitorização da ocupação dos lugares
  int lugaresOcupados = 0;

  // Lugar 1
  if (ldr1Value < threshold) {
    digitalWrite(ledVerde1, LOW);
    digitalWrite(ledVermelho1, HIGH);
    lugaresOcupados++;
    if (!lugar1Ocupado) {
      lugar1Ocupado = true;
      inicioOcupacao1 = millis();
    }
  } else {
    digitalWrite(ledVerde1, HIGH);
    digitalWrite(ledVermelho1, LOW);
    if (lugar1Ocupado) {
      lugar1Ocupado = false;
      tempoOcupacao1 += millis() - inicioOcupacao1;
      // Mostrar tempo de ocupação no LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("L1 Tempo Ocup.: ");
      lcd.setCursor(0, 1);
      lcd.print(tempoOcupacao1 / 1000);
      lcd.print("s        ");
      delay (3000);
      lcd.clear();

    }
  }

  // Lugar 2
  if (ldr2Value < threshold) {
    digitalWrite(ledVerde2, LOW);
    digitalWrite(ledVermelho2, HIGH);
    lugaresOcupados++;
    if (!lugar2Ocupado) {
      lugar2Ocupado = true;
      inicioOcupacao2 = millis();
    }
  } else {
    digitalWrite(ledVerde2, HIGH);
    digitalWrite(ledVermelho2, LOW);
    if (lugar2Ocupado) {
      lugar2Ocupado = false;
      tempoOcupacao2 += millis() - inicioOcupacao2;
      // Mostrar tempo de ocupação no LCD
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("L2 Tempo Ocup.: ");
      lcd.setCursor(0, 1);
      lcd.print(tempoOcupacao2 / 1000);
      lcd.print("s        ");
      delay(3000);
      lcd.clear();

    }
  }

  // Calcular o número de lugares livres
  int lugaresLivres = 2 - lugaresOcupados;

  // Atualizar o display LCD
  lcd.setCursor(0, 0);
  lcd.print("Lug. Livres: ");
  lcd.print(lugaresLivres);
  lcd.setCursor(0, 1);
  lcd.print("  Lug. Ocup: ");
  lcd.print(lugaresOcupados);

  
  // Verificar presença de veículo na entrada
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance < distanciaLimite) {
    // Abrir a cancela
    cancela.write(aberto);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Benvindo ao");
    lcd.setCursor(0, 1);
    lcd.print("parque GPE");
    delay(3000);
    lcd.clear();
  } else {
    // Fechar a cancela
    cancela.write(fechado);
    //lcd.setCursor(0, 1);
    //lcd.print("                 ");
  }

  delay(500);
}
