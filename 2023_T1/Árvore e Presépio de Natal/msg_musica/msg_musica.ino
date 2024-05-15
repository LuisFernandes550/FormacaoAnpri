#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd_1(0x27,16,2);

#include <Ultrasonic.h>
#define triggerPin 11
#define echoPin 12

//declarações de variaveis para a musica
int speakerPin = 9;
int length = 26;
char notes[] = "eeeeeeegcde fffffeeeeddedg";
int beats[] = { 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2};
int time1 = 300;

Ultrasonic ultrasonic(triggerPin,echoPin);
float distancia;
long tempo;

//musica
boolean ledOnOff = true;

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void playNote(char note, int duration) {
  char names[] = { 'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C' };
  int tones[] = { 1915, 1700, 1519, 1432, 1275, 1136, 1014, 956 };

  // play the tone corresponding to the note name
  for (int i = 0; i < 8; i++) {
    if (names[i] == note) {
      playTone(tones[i], duration);
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(900);
  lcd_1.init();
  pinMode(speakerPin, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  tempo=ultrasonic.timing();
  distancia=ultrasonic.convert(tempo, Ultrasonic::CM);
  Serial.print("Distância: ");
  Serial.println(distancia);
  delay(100);
  lcd_1.clear();
  if (distancia<5){
    lcd_1.backlight();
    lcd_1.setCursor(2, 0);
    lcd_1.print("Feliz Natal!");
    lcd_1.setCursor(0, 1);  
    lcd_1.print("Clube de Codigo");
    digitalWrite(5, LOW);
    //delay(1000);
     //musica
  
    for (int i = 0; i < length; i++) {
      if (notes[i] == ' ') {
      delay(beats[i] * time1); // rest
      blinky();
      }
      else {
      playNote(notes[i], beats[i] * time1);
      blinky();
      }
      delay(time1 / 2); 
    }
  }
  else {
    lcd_1.backlight();
    lcd_1.setCursor(0, 0);
    lcd_1.print("Aproxime a sua");
    lcd_1.setCursor(0, 1);  
    lcd_1.print("mao do sensor!");
    digitalWrite(5, HIGH);
    delay(500);
    digitalWrite(5, LOW);
   }
}
void blinky() {
  if (ledOnOff == true) {
    digitalWrite(2, LOW);
    digitalWrite(3, HIGH);
    ledOnOff = !ledOnOff;
  }
  else {
    digitalWrite(3, LOW);
    digitalWrite(2, HIGH);
    ledOnOff = !ledOnOff;
  }
}
