// Definição dos pins do sensor ultrasonic
const int trigPin = 7;
const int echoPin = 6;

// DDefinição dos pins dos leds
const int redPin = 12;
const int yellowPin = 11;
const int bluePin = 10;
const int greenPin = 9;
const int whitePin = 8;
const int buzzerPin = 5;

// Variáveis para o cálculo da distância
long duracao;
int distancia;

void setup() {
  // Definição dos pins do trig como saída e do echo como entrada
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Definição dos pins dos leds como de saída
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(whitePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Iniciar a comunicação série
  Serial.begin(9600);
}

void loop() {
  // Definir o pin do triger como desligado
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  
  // Definição do pin do triger como ligado por 10 microsegundos
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // leitura do echo pin, cálculo da distância
  duracao = pulseIn(echoPin, HIGH);
  distancia = duracao*0.034/2;
  
  // Imprime a distância no monitor série
  Serial.print("Distancia: ");
  Serial.print(distancia);
  Serial.println(" cm");
  
  // Controlo dos leds de acordo com a distância
  if (distancia < 10) {
    // Red LED ON, others ON
    digitalWrite(redPin, HIGH);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(bluePin, HIGH);
    digitalWrite(greenPin, HIGH);
    digitalWrite(whitePin, HIGH);
  } else if (distancia >= 10 && distancia < 30) {
    // Yellow LED ON, others ON exepto o Red
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(bluePin, HIGH);
    digitalWrite(greenPin, HIGH);
    digitalWrite(whitePin, HIGH);
  } else if (distancia >= 30 && distancia < 50) {
    // Blue LED ON, others ON exepto o yellow e o Red
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(bluePin, HIGH);
    digitalWrite(greenPin, HIGH);
    digitalWrite(whitePin, HIGH);
    } else if (distancia >= 50 && distancia < 70) {
    // Green LED ON, White led ON others OFF
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(bluePin, LOW);
    digitalWrite(greenPin, HIGH);
    digitalWrite(whitePin, HIGH);
  } else {
    // White LED ON, others OFF
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(bluePin, LOW);
    digitalWrite(greenPin, LOW);
    digitalWrite(whitePin, HIGH);
  }
  
  // Atraso antes da próxima leitura
  delay(10);

tone(5,400-distancia);
delay(distancia);
noTone(5);
delay(distancia);
}

long microsecondsToCentimeters(long microseconds){
  return microseconds / 29 / 2 ;


}