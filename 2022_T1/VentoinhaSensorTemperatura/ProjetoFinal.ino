
#include <LiquidCrystal.h> 


LiquidCrystal lcd(6, 5, 8, 9, 10, 11);
 
const int pinFan = 3; 
const int pinSensor = A5; 

float temperature; 
const int btnPin = 4;
int botao=0;
int fase=LOW;


void setup() {
  lcd.begin(16, 2); 
  lcd.clear(); 
  pinMode(pinFan, OUTPUT);
  pinMode(pinSensor, INPUT);
  pinMode(pinFan, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(botao, INPUT);
}
 
void loop() {  
    botao = digitalRead(btnPin);
  Serial.println(botao);
 if (botao==HIGH){
 fase=!fase;
 delay(1000);
	 } 
  if (fase == HIGH) {

    lcd.setCursor(0,1);
    lcd.write("Fan ligado!");
	  digitalWrite(3, HIGH);
	  digitalWrite(7, HIGH);
        }
  else {
    lcd.setCursor(0,1);
    lcd.write("Fan desligado!");
  	digitalWrite(3, LOW);
    digitalWrite(7, LOW);
          
	}
    /*Para evitar as grandes variações de leitura do componente
    LM35 são feitas 1000 leituras onde calculamos a média*/
    for (int i=0;i<1000;i++) {
      temperature = temperature + (-40 + 0.488155 * (analogRead(A5) - 20));
   }
   temperature = temperature/1000; // calcula a média das leituras

    if(temperature > 18) {
        lcd.setCursor(0,1);
        lcd.write("Fan ligado !"); 
        digitalWrite(pinFan, HIGH); 
        digitalWrite(7, HIGH);
    } else {
        lcd.setCursor(0,1);
        lcd.write("Fan desligado !"); 
        digitalWrite(pinFan, LOW); 
        digitalWrite(7, LOW);
    }
  delay(1000); 
    lcd.clear(); 
    lcd.print("Temp.: "); 
    lcd.print(temperature);
    lcd.write(B11011111); 
    lcd.print("C");

}
 



