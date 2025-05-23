#include <lcd.h>
#include <IRremote.h>

lcd Lcd;
const int RECV_PIN = 11;
const int buzzerpin = 8;  // o pin do buzzer
const int pirPin = 3;     // o pin do sensor de movimento
int pirStat = 0;
int alarme = 0;  // o estado do alarme
IRrecv irrecv(RECV_PIN);
decode_results results;


void setup() {
  pinMode(buzzerpin, OUTPUT);
  pinMode(pirPin, INPUT);
  Lcd.Init();   //inicializa o LCD
  Lcd.Clear();  
  Serial.begin(9600);
  irrecv.enableIRIn();  // inicia a receção de sinais 
}


void loop() {
  pirStat = digitalRead(pirPin);
  if (pirStat == HIGH and alarme == 1) {  // se o sensor deteta movimento e se o alarme está ativado
    digitalWrite(buzzerpin, HIGH);        // o buzzer toca
    Lcd.Clear();
    Serial.println("Alarme ligado!!!");
    Lcd.Cursor(0, 4);
    Lcd.Display("ALARME");  
    Lcd.Cursor(2, 4);
    Lcd.Display("LIGADO");
  } else {
    digitalWrite(buzzerpin, LOW);  
  }
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    switch (results.value) {
      case 0x00FF6897:  //quando se pressiona a tecla 1
        alarme = 1;
        Serial.println("Alarme ATIVADO!!!");
        Lcd.Clear();
        Lcd.Cursor(0, 4);
        Lcd.Display("ALARME");  
        Lcd.Cursor(2, 4);
        Lcd.Display("ATIVADO");
        break;
      case 0x00FF9867:  //quando se pressiona a tecla 2
        pirStat = 0;
        alarme=0;
        Serial.println("Alarme DESATIVADO!!!");
        Lcd.Clear();
        Lcd.Cursor(0, 4);
        Lcd.Display("ALARME");  
        Lcd.Cursor(2, 4);
        Lcd.Display("DESATIVADO");
        break;
    }
    irrecv.resume();  // recebe o próximo valor
  }
  delay(100);
}