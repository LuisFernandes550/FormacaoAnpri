
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int ledPinRed = 7;
const int ledPinYellow = 10;
const int ledPinGreen = 13;
const int bombaPin = 3;

const int tmpPin = A0;

int sensor, pot;

void setup()
{
  // put your setup code here, to run once:
  pinMode(bombaPin, OUTPUT);

  pinMode(ledPinRed, OUTPUT);
  pinMode(ledPinYellow, OUTPUT);
  pinMode(ledPinGreen, OUTPUT);
  pinMode(tmpPin, INPUT);
  lcd.init();
  lcd.backlight();

  digitalWrite(bombaPin, LOW);

  Serial.begin(9600);
}

void loop()
{
  // put your main code here, to run repeatedly:
  delay(2000);

  sensor = analogRead(tmpPin);
  float humidade = sensor / 10;

  lcd.setCursor(0, 0);
  lcd.print("  Planta 1 ");
  lcd.setCursor(0, 1);
  lcd.print("Humidade ");
  lcd.print(humidade);
  lcd.print((char)223);
  lcd.print("% ");

  // Pouca Humidade 25%
  if (humidade < 25)
  {

    digitalWrite(ledPinRed, HIGH);
    digitalWrite(ledPinYellow, LOW);
    digitalWrite(ledPinGreen, LOW);
    // motor ligado
    digitalWrite(bombaPin, HIGH);
    Serial.println(bombaPin);
    delay(2000);
  }
  // Alguma humidade 25% a 50%
  else if (humidade >= 25 && humidade < 50)
  {
    digitalWrite(ledPinRed, LOW);
    digitalWrite(ledPinYellow, HIGH);
    digitalWrite(ledPinGreen, LOW);
    // motor ligado
    digitalWrite(bombaPin, HIGH);
    Serial.println(bombaPin);
    delay(2000);
  }
  // Humidade adequada 50% a 100%
  else
  {
    digitalWrite(ledPinRed, LOW);
    digitalWrite(ledPinYellow, LOW);
    digitalWrite(ledPinGreen, HIGH);
    // motor desligado
    digitalWrite(bombaPin, LOW);
    Serial.println(bombaPin);
    delay(2000);
  }
}
