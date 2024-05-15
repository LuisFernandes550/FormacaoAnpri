const int buzzerPin = 13;  //variavel constante para o pino digital referente ao buzzer
#define ledPin 4           //led vermelho
#define ledPin2 5          //led vermelho
#define ledEstrela 6       //led amarelo
#define ledPin3 3          //led verde
#define ledPin4 2          //led verde

//parte da melodia jingle bells
int length = 38;
char notes[] = "eeeeeeegcde fffffeeeeddedg eeeeeeegcde";
int beats[] = { 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 4 };

int tempo = 300;

boolean ledOnOff = true;

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);
  pinMode(ledEstrela, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
}

void loop() {
  for (int i = 0; i < length; i++) {
    tocaNota(notes[i], beats[i] * tempo);
    piscaLeds();
    delay(tempo / 2);  //pausa entre notas
  }
}

void tocaNota(char nota, int duracao) {
  char nome[] = { 'c', 'd', 'e', 'f', 'g' };
  int tons[] = { 650, 600, 550, 500, 450 };

  // toca o tom correspondente ao nome da nota
  for (int i = 0; i < 5; i++) {
    if (nome[i] == nota) {
      tocaTom(tons[i], duracao);
    }
  }
}

//piscar dos leds
//o led amarelo fica sempre ligado pois simula a estrela no topo da árvore
void piscaLeds() {
  if (ledOnOff == true) {
    digitalWrite(ledPin, HIGH);  //acende o led
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, HIGH);
    digitalWrite(ledEstrela, HIGH);
    ledOnOff = !ledOnOff;  //passa a false

  } else {

    digitalWrite(ledPin, LOW);  //desliga o led
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, LOW);
    ledOnOff = !ledOnOff;  //passa a true
  }
}

void tocaTom(int tom, int duracao) {
  //duracao *1000L para declarar que duracao é long e não int
  for (long i = 0; i < duracao * 1000L; i = i + tom * 2) {
    digitalWrite(buzzerPin, HIGH);
    delayMicroseconds(tom);
    digitalWrite(buzzerPin, LOW);
    delayMicroseconds(tom);
  }
}



