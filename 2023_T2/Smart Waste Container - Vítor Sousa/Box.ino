#include <FastLED.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>
#include <Servo.h>
Servo myservo;
#define NUM_LEDS 20 // Número de LEDs na fita
#define DATA_PIN 9   // Pino de dados da fita de LEDs
#define TRIGGER_PIN 4 // Pino de disparo do sensor ultrassom
#define ECHO_PIN 5    // Pino de eco do sensor ultrassom
#define MAX_DISTANCE 39 // Distância máxima do sensor em centímetros
#define LED_BRIGHTNESS  255 // Brilho dos LEDs (0-255)

const int MH1 = 2;      //sensor1
const int MH2 = 3;      //sensor2
int VH1;                //variavel 1 do sensor 1
int VH2;                //variavel 2 do sensor 2
float cm;               //variavel em cm
int w;
int sensorPin = 7;
int val;
int servo = 6;
LiquidCrystal_I2C lcd(0x27, 16, 2);
Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);

CRGB leds[NUM_LEDS];

void setup() {
  Serial.begin(9600);
  pinMode(MH1, INPUT);
  pinMode(MH2, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(LED_BRIGHTNESS);

  pinMode (sensorPin, INPUT);
  myservo.attach(servo);
 
  lcd.begin();
  lcd.backlight();
}

void loop() {
  myservo.write(0);
  long duration, distance;

  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  duration = pulseIn(ECHO_PIN, HIGH);

  // Calcula a distância em centímetros
  distance = (duration / 2) / 29.1;

  //Serial.print("Distancia: ");
  //Serial.println(distance);

  // Mapeia a distância para o número de LEDs acesos
  int numLedsAcesos = map(distance, 0, MAX_DISTANCE, 0, NUM_LEDS);

  // Acende os LEDs de acordo com a distância
  for (int i = 0; i < NUM_LEDS; i++) {
    if (distance > MAX_DISTANCE) {
      leds[i] = CRGB::Black; // Cor dos LEDs apagados se a distância for maior que 41 cm
    } else {
      if (i < numLedsAcesos) {
        if (distance >= 1 && distance <= 10) {
          leds[i] = CRGB::Red; // Cor vermelha para distância entre 1 e 10
        } else if (distance >= 11 && distance <= 26) {
          leds[i] = CRGB::Yellow; // Cor amarela para distância entre 11 e 26
        } else if (distance >= 27 && distance <= 41) {
          leds[i] = CRGB::Green; // Cor verde para distância entre 27 e 41
        }
      } else {
        leds[i] = CRGB::Black; // Cor dos LEDs apagados
      }
    }
  }
  FastLED.show();
  delay(100); // Ajuste conforme necessário

  // Atualiza o LCD com informações dos sensores de capacidade e peso
  lcd.setCursor(0, 0);
  lcd.print("Capacity:");
  lcd.setCursor(0, 1);
  lcd.print("Weight:");
  VH1 = digitalRead(MH1);
  VH2 = digitalRead(MH2);
  cm = ultrasonic.distanceRead(CM);
  val = digitalRead(sensorPin);

  if (VH2 == 0 && VH1 == 1) {
    lcd.setCursor(9, 0);
    lcd.print("Medium");
  } else if (VH1 == 0) {
    lcd.setCursor(9, 0);
    lcd.print("Full  ");
  } else {
    lcd.setCursor(9, 0);
    lcd.print("Empty ");
  }
  cm = 40 - cm;
  float g = (cm - 2) * 0.0303030303;
  //Serial.println(g);
  if (g >= 1) {
    w = g * 1000;
    lcd.setCursor(7, 1);
    lcd.print("1kg   ");
  } else if (g < 0) {
    lcd.setCursor(7, 1);
    lcd.print("0g     ");
  } else {
    w = g * 1000;
    lcd.setCursor(7, 1);
    lcd.print(w);
    lcd.print("g");
  }
  if (val == HIGH) {
    Serial.println("abrir");
    myservo.write(180);
    delay(5000);
    myservo.write(0);
  }
  Serial.println(VH1);
  Serial.println(VH2);
  Serial.println("------");
}