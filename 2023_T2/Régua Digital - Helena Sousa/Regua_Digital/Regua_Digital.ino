#include <NewPing.h>
#include <LCD_I2C.h>

#define TRIGGER_PIN  13  
#define ECHO_PIN     12  
#define MAX_DISTANCE 200  
#define ledverde 7
#define ledavermelho 8
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 
LCD_I2C lcd(0x27); 


void setup()
{
    lcd.begin(); 
    lcd.backlight();
    Serial.begin(9600);
    pinMode(ledverde, OUTPUT);
    pinMode(ledavermelho, OUTPUT);
}

void loop()
{      
    int dist=sonar.ping_cm();        
    Serial.print("Ping: ");
    Serial.print(dist); 
    Serial.println("cm");
    lcd.setCursor(1, 0);
    lcd.print("Projeto Final"); 
    lcd.setCursor(0, 1); 
    lcd.print("Distancia:");
    lcd.print(dist);
    lcd.print(" cm");
    if (dist >=0 && dist<=10){
       digitalWrite(ledverde, HIGH);
       digitalWrite(ledavermelho, LOW);
    }
    else{
       digitalWrite(ledavermelho, HIGH);
       digitalWrite(ledverde, LOW);
    }
    delay(500);
}
