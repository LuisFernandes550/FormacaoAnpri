#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27,16,2);

#define DHTPIN 7
#define DHTTYPE DHT11

DHT dht(DHTPIN,DHTTYPE);

int BUZZER = 10; 
int led_normal = 11; 
int led_alarme = 12;
int sensor_gas; 

void setup() { 
  dht.begin();
  lcd.init();

  pinMode(led_normal,OUTPUT); 
  pinMode(led_alarme,OUTPUT); 
  pinMode(BUZZER,OUTPUT);
  Serial.begin(9600); 
}
void loop() {
sensor_gas = analogRead(A0);
if (sensor_gas > 900){
// existe gás
digitalWrite(led_alarme,HIGH);
digitalWrite(led_normal,LOW);
tone(BUZZER,2700);
delay(200);
noTone(BUZZER);

}
else{
// Todo normal digitalWrite(lednormal,HIGH);
digitalWrite(led_alarme,LOW); 
digitalWrite(led_normal,HIGH); 
noTone(BUZZER);
}
  //delay(2000);

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  lcd.backlight(); 
  lcd.setCursor(0, 0);
  lcd.print("Humidade: "); 
  lcd.print(h); lcd.print(" % ");
  lcd.setCursor(0, 1);
  lcd.print("Temp: "); 
  lcd.print(t); 
  lcd.print((char) 223); 
  lcd.print("C "); 



}
