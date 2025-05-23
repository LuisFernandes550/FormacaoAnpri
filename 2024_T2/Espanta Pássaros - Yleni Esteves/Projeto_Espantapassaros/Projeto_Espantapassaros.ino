int buzzerPin = 8;    // Buzzer
int ldrPin = A0;      // LDR
int ldrValor = 0;
int limiteLuz = 1019;  // Limiar para considerar dia/noite

int led1Pin = 9;      // LED 1
int led2Pin = 10;     // LED 2
int led3Pin = 11;     // LED 3

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  pinMode(led3Pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  ldrValor = analogRead(ldrPin);
  Serial.println(ldrValor);

  if (ldrValor > limiteLuz) {
    // É dia, toca buzzer e apaga LEDs
    for (int i = 0; i < 3; i++) {
      tone(buzzerPin, 1000);
      delay(500);
      noTone(buzzerPin);
      delay(500);
    }
    // Apaga todos os LEDs
    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, LOW);
  } else {
    // É Noite, o Buzzer é desligado e os LEDs piscam alternadamente
    noTone(buzzerPin);

    digitalWrite(led1Pin, HIGH);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, LOW);
    delay(300);

    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, HIGH);
    digitalWrite(led3Pin, LOW);
    delay(300);

    digitalWrite(led1Pin, LOW);
    digitalWrite(led2Pin, LOW);
    digitalWrite(led3Pin, HIGH);
    delay(300);
  }

  delay(500);  // delay antes da próxima leitura
}