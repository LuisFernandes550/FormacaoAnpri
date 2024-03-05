//Programa: Monitorização de planta usando Arduino 
#define SCL A5
#define SDA A4
#define pino_sinal_analogico A0
#define pino_led_vermelho 5
#define pino_led_amarelo 6
#define pino_led_verde 7

#include <Wire.h> 

#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT11 
                      
DHT dht(DHTPIN, DHTTYPE); 
 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
 
int valor_analogico;
 
void setup()
{
  Serial.begin(9600);
  pinMode(pino_sinal_analogico, INPUT);
  pinMode(pino_led_vermelho, OUTPUT);
  pinMode(pino_led_amarelo, OUTPUT);
  pinMode(pino_led_verde, OUTPUT);
  dht.begin(); 
  lcd.backlight(); 
  lcd.init(); 
}
 
void loop()
{
   // valor de Humidade e Temperatura Ar
   lcd.clear();
   lcd.setCursor(0,0); 
   lcd.print("Humidity= ");
   lcd.print((float)dht.readHumidity()); 
   lcd.print("%");
   lcd.setCursor(0,1); 
   lcd.print("Temp= ");
   lcd.print((float)dht.readTemperature()); 
   lcd.print(" C");
   delay(2000);
   lcd.clear();
       
    
    //Le o valor do pino A0 do sensor
  valor_analogico = analogRead(pino_sinal_analogico);
 
  //Mostra o valor da porta analogica no serial monitor
  Serial.print("Porta analogica: ");
  Serial.print(valor_analogico);
 
  //Solo humido, acende o led verde
  if (valor_analogico > 0 && valor_analogico < 300)
  {
    Serial.println(" Status: Solo humido");
    apagaleds();
    digitalWrite(pino_led_verde, HIGH);
  }
 
  //Solo com humidade moderada, acende led amarelo
  if (valor_analogico > 310 && valor_analogico < 600)
  {
    Serial.println(" Status: Humidade moderada");
    apagaleds();
    digitalWrite(pino_led_amarelo, HIGH);
  }
 
  //Solo seco, acende led vermelho
  if (valor_analogico > 600 && valor_analogico < 700)
  {
    Serial.println(" Status: Solo seco");
    apagaleds();
    digitalWrite(pino_led_vermelho, HIGH);
  }
  delay(500);
}
 
void apagaleds()
{
  digitalWrite(pino_led_vermelho, LOW);
  digitalWrite(pino_led_amarelo, LOW);
  digitalWrite(pino_led_verde, LOW);
}
