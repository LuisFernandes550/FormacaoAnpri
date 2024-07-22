#include<Servo.h> // Biblioteca do servomotor 
Servo servo; //Define o objeto servomotor

#include <Ultrasonic.h> // Biblioteca do sensor ultrasonico
#define triggerPin 10 // Pino trigger do sensor ultrasonico
#define echoPin 9 // Pino echo do sensor ultrasonico

Ultrasonic ultrasonic(triggerPin, echoPin); //Define o objeto do sensor ultrasonico

float distancia; 
long tempo;

#define pinoLed1 2 //Pino do led vermelho 
#define pinoLed2 3  //Pino do led verde  
#define pinoSensor 4 //Pino do sensor


void setup()
  {
  pinMode(triggerPin, OUTPUT); // Define o pino trigger como saída
  pinMode(echoPin, INPUT); // Define o pino echo como entrada
  servo.attach(8);// Pino do servomotor

  pinMode(pinoSensor, INPUT); // Define o pino do sensor como entrada
  pinMode(pinoLed1, OUTPUT); // Define o pino do led vermelho como saída 
  pinMode(pinoLed2, OUTPUT); // Define o pino do led verde como saída      
  digitalWrite(pinoLed1, LOW); // Começa led vermelho desligado
  digitalWrite(pinoLed2, HIGH); // Começa led verde ligado
  }

void loop()
{       
  tempo = ultrasonic.timing();
  distancia = ultrasonic.convert(tempo, Ultrasonic::CM);
  delay(200);

  if (distancia <= 50 && distancia >= 0) // Se a distância for entre o cm e 50 cm
  {  
   servo.write(0); // Abre o caixote
   delay(3000);
  } 
  else 
  {   
   servo.write(90);// Fecha O caixote
  }
 
  if (digitalRead(pinoSensor) == LOW) //Se o sensor não detetar lixo
  { 
      digitalWrite(pinoLed1, LOW); //Desliga o led vermelho
      digitalWrite(pinoLed2, HIGH); //Liga o led verde
  }
  else //Se o sensor detetar lixo
  {
    digitalWrite(pinoLed1, HIGH); //Liga o led vermelho
    digitalWrite(pinoLed2, LOW); //Desliga o led verde
  }   
}
