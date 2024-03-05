// #include <Ultrasonic.h>

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);  //LCD no endereco 0x27

int sequencia[32];
int test[] = { 5, 4, 3, 2 };
int seq = 0;
#define buzzer 7
#define red 2
#define green 3
#define yellow 4
#define blue 5
int btnStateRed = 0;
int btnStateGreen = 0;
int btnStateYellow = 0;
int btnStateBlue = 0;
int dificuldade = 2;
int pontos = 0;
bool acertaBtn = true;
// #define triggerPino 11
// #define echoPino 12
// Ultrasonic ultrasonic(triggerPino, echoPino);
// float distancia;
// long tempo;

void setup() {
  // inicio do jogo
  pinMode(buzzer, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  digitalWrite(red, HIGH);
  digitalWrite(green, HIGH);
  digitalWrite(yellow, HIGH);
  digitalWrite(blue, HIGH);
  delay(500);
  digitalWrite(red, LOW);
  digitalWrite(green, LOW);
  delay(500);
  digitalWrite(blue, LOW);
  digitalWrite(yellow, LOW);
  digitalWrite(red, HIGH);
  digitalWrite(blue, HIGH);
  delay(500);
  digitalWrite(red, LOW);
  digitalWrite(blue, LOW);
  digitalWrite(yellow, HIGH);
  digitalWrite(green, HIGH);
  delay(500);

  //iniciar o lcd
  lcd.backlight();
  lcd.init();
  lcd.print("Dific-4 Facil-1");  //jogador pode escolher a dificuldade do jogo
  lcd.setCursor(0, 1);
  lcd.print("4vr 3vd 2am 1az");
  pinMode(red, INPUT);
  pinMode(yellow, INPUT);
  pinMode(green, INPUT);
  pinMode(blue, INPUT);
  //espera que a seleção da dificuldade seja feita
  while (btnStateRed == 0 && btnStateGreen == 0 && btnStateYellow == 0 && btnStateBlue == 0) {
    btnStateRed = digitalRead(red);
    if (btnStateRed == HIGH) {
      dificuldade = 4;
      break;
    }

    btnStateGreen = digitalRead(green);
    if (btnStateGreen == HIGH) {
      dificuldade = 3;
      break;
    }
    btnStateYellow = digitalRead(yellow);
    if (btnStateYellow == HIGH) {
      dificuldade = 2;
      break;
    }

    btnStateBlue = digitalRead(blue);
    if (btnStateBlue == HIGH) {
      dificuldade = 1;
      break;
    }
  }
  lcd.setCursor(0, 1);
  lcd.clear();
  delay(1500);
  //Serial.begin(9600);
}

void loop() {
  geraSequencia();          //função que gera a sequencia de cores na ronda
  verificaSeq();            //função que verifica se o jogador acertou na sequencia
  if (acertaBtn == true) {  //se acerta na sequencia ganha pontos conforme a dificuldade
    pontos += dificuldade * 100;
    dificuldade *= 2;
    lcd.setCursor(0, 0);
    lcd.print("Fantastico :)");
    lcd.setCursor(0, 1);
    lcd.print("Pontos--> ");
    lcd.print(pontos);
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ronda seguinte");
    lcd.setCursor(0, 1);
    lcd.print("Pontos--> ");
    lcd.print(pontos);
    delay(2000);
    lcd.clear();
    delay(3000);
  } else {
    delay(2000);
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Carregue no azul ");
    lcd.setCursor(0, 1);
    lcd.print("para jogar :)");

    // tempo = ultrasonic.timing();                            //tempo ida e volta do sinal
    // distancia = ultrasonic.convert(tempo, Ultrasonic::CM);  //determina a distancia ao objeto
    // if (distancia < 100) {
    //   delay(100); //aguarda que o utilizador coloque a mão próxima do sensor
    //   resetBtnState();
    // }

    delay(100);
    resetBtnState();           //reinicia botões
    while (btnStateBlue == 0)  //espera que utilizador carregue no botão azul
    {
      btnStateBlue = digitalRead(blue);
    }
    lcd.print("Dific-4 Facil-1");  //reinicia jogo
    lcd.setCursor(0, 1);
    lcd.print("4vr 3vd 2am 1az");
    delay(1500);
    resetBtnState();
    acertaBtn = true;
    //jogador define a dificuldade do jogo
    while (btnStateRed == 0 && btnStateGreen == 0 && btnStateYellow == 0 && btnStateBlue == 0) {
      btnStateRed = digitalRead(red);
      if (btnStateRed == HIGH) {
        dificuldade = 4;
        pontos = 0;
        delay(1500);
        lcd.clear();
        break;
      }

      btnStateGreen = digitalRead(green);
      if (btnStateGreen == HIGH) {
        dificuldade = 3;
        pontos = 0;
        delay(1500);
        lcd.clear();
        break;
      }

      btnStateYellow = digitalRead(yellow);
      if (btnStateYellow == HIGH) {
        dificuldade = 2;
        pontos = 0;
        delay(1500);
        lcd.clear();
        break;
      }

      btnStateBlue = digitalRead(blue);
      if (btnStateBlue == HIGH) {
        dificuldade = 1;
        pontos = 0;
        delay(1500);
        lcd.clear();
        break;
      }
    }
  }
}

void geraSequencia() {  //função que vai gerar sequencia de cores
  lcd.setCursor(0, 1);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  for (int i = 0; i < dificuldade; i++) {
    sequencia[i] = random(2, 6);
    digitalWrite(sequencia[i], HIGH);  //acende o led e o som da cor correspondente
    if (sequencia[i] == 2) {
      tone(buzzer, 440);
      delay(100);
      noTone(buzzer);
    }
    if (sequencia[i] == 3) {
      tone(buzzer, 340);
      delay(100);
      noTone(buzzer);
    }
    if (sequencia[i] == 4) {
      tone(buzzer, 380);
      delay(100);
      noTone(buzzer);
    }
    if (sequencia[i] == 5) {
      tone(buzzer, 400);
      delay(100);
      noTone(buzzer);
    }
    delay(500);
    digitalWrite(sequencia[i], LOW);
    delay(500);
  }
  pinMode(red, INPUT);
  pinMode(yellow, INPUT);
  pinMode(green, INPUT);
  pinMode(blue, INPUT);
}

void verificaSeq() {  //função que verifica se o utilizador acerta na sequencia
  resetBtnState();
  for (int i = 0; i < dificuldade; i++) {
    seq = sequencia[i];
    //espera que o utilizador pressione os botões
    while (btnStateRed == 0 && btnStateGreen == 0 && btnStateYellow == 0 && btnStateBlue == 0) {

      btnStateRed = digitalRead(red);
      if (btnStateRed == HIGH && seq != 2) {
        acertaBtn = false;
      }
      if (btnStateRed == HIGH && seq == 2) {
        tone(buzzer, 440);
        delay(100);
        noTone(buzzer);
      }

      btnStateGreen = digitalRead(green);
      if (btnStateGreen == HIGH && seq != 3) {
        acertaBtn = false;
      }
      if (btnStateGreen == HIGH && seq == 3) {
        tone(buzzer, 340);
        delay(100);
        noTone(buzzer);
      }

      btnStateYellow = digitalRead(yellow);
      if (btnStateYellow == HIGH && seq != 4) {
        acertaBtn = false;
      }
      if (btnStateYellow == HIGH && seq == 4) {
        tone(buzzer, 380);
        delay(100);
        noTone(buzzer);
      }

      btnStateBlue = digitalRead(blue);
      if (btnStateBlue == HIGH && seq != 5) {
        acertaBtn = false;
      }
      if (btnStateBlue == HIGH && seq == 5) {
        tone(buzzer, 400);
        delay(100);
        noTone(buzzer);
      }
    }
    delay(500);
    if (acertaBtn == 0)  //se errou no botão
    {
      lcd.setCursor(0, 0);
      lcd.print("*** ERROU  ***");
      lcd.setCursor(0, 1);
      lcd.print("PERDEU O JOGO!");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("RESULTADO FINAL: ");  //mostra o resultado final do jogador
      lcd.setCursor(0, 1);
      lcd.print(pontos);
      break;
    }
    resetBtnState();  //chama a função que vai reiniciar o estado dos botões
  }
}

void resetBtnState()  //função que vai reiniciar o estado dos botões
{
  btnStateRed = 0;
  btnStateGreen = 0;
  btnStateYellow = 0;
  btnStateBlue = 0;
}
