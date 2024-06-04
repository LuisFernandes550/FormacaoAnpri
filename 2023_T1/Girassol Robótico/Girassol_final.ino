#include <Servo.h>

int sensorPin = A0;
int servoPin  = 9;

int sensorValue = 0;
int servoGrad = 90;
int tolerance = 20;
int meio;

Servo myservo;

void setup() {
  Serial.begin(9600);
  pinMode( sensorPin, INPUT);
  myservo.attach( servoPin );
  myservo.write( servoGrad );
  meio = analogRead(sensorPin);
}
void loop() {
  sensorValue = analogRead(sensorPin);
  Serial.println(sensorValue);
  Serial.println(meio);
  if ( sensorValue < (meio-tolerance) )
  {
    if (servoGrad <= 180) servoGrad--;
  }

  if ( sensorValue >= (meio+tolerance) )
  {
    if (servoGrad >= 0) servoGrad++;
  }

  myservo.write( servoGrad );
  Serial.println(servoGrad);
  Serial.println("-----------------");

  delay(100);
}