#include <Ultrasonic.h>
#include <Servo.h>

Servo myservo;  
Ultrasonic ultrasonic(2, 3); // Definição dos pins que fazem echo e trigger
int distancia;

void setup() {
  myservo.attach(10);  // Define o motor servo no pin 10
  Serial.begin(9600);
  myservo.write(0);
}

void loop() {
  
  distancia = ultrasonic.read();
  if (distancia <= 10){            // 10 é a distância para detetar objetos/pessoas que pretendem usar o caixote do lixo
    myservo.write(80);            //  angulo do braço do motor servo
    delay(5000);
    myservo.write(0);             // fecha a tampa após 5 segundos
  }
  Serial.print("Distância : ");  // controlo da distância no monitor serial
  Serial.println(distancia);
  delay(100);
}

