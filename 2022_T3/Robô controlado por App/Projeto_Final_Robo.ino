#include <SPI.h>
#include <RFID.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>

// MOTORES
const int enA = 10, in1 = 9, in2 = 8, in3= 7, in4 = 6, enB = 5;
// LEDS
const int ledVerde = 3;
const int ledVermelho = 11;
// BUZZER
const int pinBuzzer = 2;
// RFID
//SDA (SS) E RST (reset) pins
const int SDA_DIO = 13;
const int RESET_DIO = 12;

RFID RC522(SDA_DIO, RESET_DIO); 

const int triggerPin = 4; 
const int echoPin = 49;

Ultrasonic ultrasonic(triggerPin, echoPin); 
float distancia; 
long tempo;

char opcao;
int velocidadeMotorA, velocidadeMotorB;

LiquidCrystal_I2C lcd(0x27,16,2);

//SoftwareSerial bluetooh (2,3); //RX,TX

int login = 0;

void setup() {

  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(enB, OUTPUT);
  
  pinMode(ledVerde, OUTPUT);
  pinMode(ledVermelho, OUTPUT);
  pinMode(pinBuzzer, OUTPUT);

  /* Ativar a SPI interface */
  SPI.begin(); 
  /* Inicializar o RFID reader */
  RC522.init();
  /* Inicializar o LCD */
  lcd.init();
  Serial.begin(9600);
  //bluetooh.begin(9600);
  Serial1.begin(9600);
}

void loop() {
  
  lcd.backlight();
  lcd.clear(); 
  lcd.setCursor(0, 0);
  String numero_cartao = "";

  if(login == 0) {
    lcd.print("Login:");
   /* Deteção do cartão RFID */
  if (RC522.isCard())
  {
    /* Leitura do número do cartão */
    RC522.readCardSerial();
    Serial.println("Cartão detetado:");
    lcd.clear(); 
    lcd.print("Cartao detetado:");
    for(int i=0;i<5;i++)
    {
    Serial.print(RC522.serNum[i],DEC);
    numero_cartao.concat(String(RC522.serNum[i], DEC));
    //Serial.print(RC522.serNum[i],HEX); //to print card detail in Hexa Decimal format
    }
    lcd.setCursor(0, 1);
    lcd.print(numero_cartao);
    delay(1000);
    lcd.clear(); 
    lcd.setCursor(0, 1);
    if(numero_cartao == "22714356146198" || numero_cartao == "21139151148247"){
        login = 1;
        Serial.println("Autenticação correta");
        lcd.print("Login OK!");
         delay(2000);
    }
    Serial.println();
  }
  }
  /* LOGIN OK */
  if(login == 1) {

  lcd.clear(); 
  lcd.setCursor(0, 0);
  clearLCDLine(0);
  //lcd.print("Estado do Carro");
  delay(100);

  tempo = ultrasonic.timing(); 
  distancia = ultrasonic.convert(tempo, Ultrasonic::CM);
  if(distancia < 6 ) {
     digitalWrite(ledVermelho, HIGH); 
     tone(pinBuzzer, 260); 
     delay(200);
     noTone(pinBuzzer);
     digitalWrite(ledVermelho, LOW); 
     moveStop();
  }

  Serial.print("Distância: "); 
  Serial.println(distancia);
  clearLCDLine(0); 
  lcd.print("Dist: ");
  lcd.print(distancia);
  lcd.print(" cm");
  delay(500);

   // if(Serial.available()){
   //if(bluetooh.available()){
    if(Serial1.available()){

    // opcao = Serial.read();
    //opcao = bluetooh.read();
    opcao = Serial1.read();
    Serial.println(opcao);

      switch (opcao) {
      case 'F':
              digitalWrite(ledVerde, HIGH);
              digitalWrite(ledVermelho, LOW);    
              moveF();
              clearLCDLine(1);
              lcd.print("Frente");
              Serial.println("Frente");
              delay(200);
              break;
      case 'B': 
              digitalWrite(ledVerde, LOW);
              digitalWrite(ledVermelho, HIGH); 
              tone(pinBuzzer, 260); 
              moveB();
              clearLCDLine(1);
              lcd.print("Marcha Atrás");
              Serial.println("Marcha Atrás");
              delay(200);
              noTone(pinBuzzer);
              break;
      case 'L':
              digitalWrite(ledVerde, HIGH);
              digitalWrite(ledVermelho, LOW);    
              moveL();
              clearLCDLine(1);
              lcd.print("Esquerda");
              Serial.println("Esquerda");
              delay(200);
              break;
      case 'R':
              digitalWrite(ledVerde, HIGH);
              digitalWrite(ledVermelho, LOW);   
              moveR();
              clearLCDLine(1);
              lcd.print("Direita");
              Serial.println("Direita");
              delay(200);
              break;      
      case 'S':
              digitalWrite(ledVerde, LOW);
              digitalWrite(ledVermelho, LOW); 
              moveStop();
              clearLCDLine(1);
              lcd.print("Parar");
              Serial.println("Parar");
              delay(200);
              break;
      case 'q':
              clearLCDLine(1);
              lcd.print("Sair...");
              Serial.println("Sair...");
              delay(200); 
              break;
      }

  }
}
  
}

  void moveF(){
     // Velocidade do Motar A e Motor B
    controlSpeed(255, 255);

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }

    void moveB(){
 // Velocidade do Motar A e Motor B
    controlSpeed(255, 255);

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }

      void moveL(){
 // Velocidade do Motar A e Motor B
    controlSpeed(255, 255);

    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
  }

   void moveR(){
 // Velocidade do Motar A e Motor B
    controlSpeed(255, 255);

    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
  }

  void moveStop(){
    // Velocidade do Motar A e Motor B
    controlSpeed(0, 0);

    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
  }
  void controlSpeed(int vA, int vB)
  {
    // para controlar a velocidade
    //Motor A
     analogWrite(enA, vA); // velocidade entre 0 e 255
    //Motor B
     analogWrite(enB, vB); // velocidade entre 0 e 255
  }

void clearLCDLine(int line){
  for(int n = 0; n < 20; n++) { 
  lcd.setCursor(n,line);
  lcd.print(" ");
  }
  lcd.setCursor(0,line); 
}





