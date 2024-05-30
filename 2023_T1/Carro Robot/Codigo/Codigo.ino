#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2); 

#include <Ultrasonic.h>
Ultrasonic ultrasonic(2,3); // (Trig PIN,Echo PIN)

//definição dos pinos do módulo L298N
 const int motorPin1 = 9; 
 const int motorPin2 = 10; 
 const int motorPin3 = 6; 
const int motorPin4 = 5;
//definição dos pinos do interruptor para modo automático
int modo = 12;
int modoControlo;
 
void setup() {
{
// iniciação do lcd 
lcd.init();                      
// Mostrar mensagem no LCD
lcd.backlight();
lcd.setCursor(0,0);
lcd.print("#Clube Robotica#");
lcd.setCursor(2,1);
lcd.print("Inscreve-te");
}
//iniciação dos pinos do motor
pinMode(motorPin1, OUTPUT);
pinMode(motorPin2, OUTPUT);
pinMode(motorPin3, OUTPUT);
pinMode(motorPin4, OUTPUT);
Serial.begin(9600);
}

void loop(){

//interruptor para selecionar o modo
modoControlo = digitalRead(modo); 
if (modoControlo == HIGH)
{
controloAutomato();
}
else
{
controloBluetooth();
}
}
// Funcao automatico quando selecionada, o robot desvia-se sozinho dos obstaculos
void controloAutomato(){ 
if( ultrasonic.Ranging(CM) >= 8)
{
analogWrite(motorPin1, 0 );
analogWrite(motorPin2, 150);
analogWrite(motorPin3, 160);
analogWrite(motorPin4, 0);
}
else{
//Recuar durante 900 ms
analogWrite(motorPin1, 150);
analogWrite(motorPin2, 0);
analogWrite(motorPin3, 0);
analogWrite(motorPin4, 160);
delay(900);
//Virar para a direita durante 500ms
analogWrite(motorPin1, 150);
analogWrite(motorPin2, 0);
analogWrite(motorPin3, 160);
analogWrite(motorPin4, 0);
Serial.println("Esquerda");
delay(500);
}
}
// Funcao manual ativada, ele aguarda comandos enviados pelo smartphone para se movimentar
 void controloBluetooth()
{ 
if(Serial.available() > 0)
{
char data;
data = Serial.read();
Serial.write(Serial.read());
analogWrite(motorPin1, 0);
analogWrite(motorPin2, 0);
analogWrite(motorPin3, 0);
analogWrite(motorPin4, 0);
switch (data)
{
case 'F': //Avançar
analogWrite(motorPin1, 0);
analogWrite(motorPin2, 150);
analogWrite(motorPin3, 160);
analogWrite(motorPin4, 0);
break;
case 'B': //Recuar
analogWrite(motorPin1, 150);
analogWrite(motorPin2, 0);
analogWrite(motorPin3, 0);
analogWrite(motorPin4, 160);
break;
case 'L': //avançar esquerda
analogWrite(motorPin1, 0);
analogWrite(motorPin2, 150);
analogWrite(motorPin3, 0);
analogWrite(motorPin4, 0);
break;
case 'R': //avançar direita
analogWrite(motorPin1, 0);
analogWrite(motorPin2, 0);
analogWrite(motorPin3, 160);
analogWrite(motorPin4, 0);
break;
default: //parado
analogWrite(motorPin1, 0);
analogWrite(motorPin2, 0);
analogWrite(motorPin3, 0);
analogWrite(motorPin4, 0);
}
}
{
}
}
