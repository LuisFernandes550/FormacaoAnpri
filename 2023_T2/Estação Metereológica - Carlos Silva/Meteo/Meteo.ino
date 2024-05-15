

#include "DHT.h"
#define DHTPIN 7     
#define DHTTYPE DHT11   

#include <LiquidCrystal_I2C.h>  
LiquidCrystal_I2C lcd(0x27,20,2);  

DHT dht(DHTPIN, DHTTYPE); 

void setup() {
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht.begin();

  lcd.init();                              
  lcd.backlight();  
}

void loop() {
  
  lcd.clear();
  lcd.setCursor ( 0, 0 );            
  lcd.print("ES METERIOLOGICA"); 
  lcd.setCursor ( 0, 1 );           
  lcd.print("PAP 3TE 2023/24"); 
  
  delay(2000);
  
  
  float h = dht.readHumidity();

  float t = dht.readTemperature();

  float f = dht.readTemperature(true);


  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  String message = (String) "Humidity: "+h+"%  Temperature: " +t+"°C";
  Serial.println(message);

  lcd.clear();
  lcd.setCursor ( 0, 0 );                     
  lcd.print("Temperatura  ");                 
  
  int intt=t;
  lcd.print(intt);                               
  lcd.println("C  ");                         
  lcd.setCursor ( 0, 1 );                     
  
  int inth=h;
  lcd.print("Humidade     ");                 
  lcd.print(inth);                               
  lcd.println("%");      

  delay(2000);
}
