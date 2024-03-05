#include <Stepper.h>

//Carregar a bliblioteca para usar o LCD e inializar a Variável
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//Inicializa as variáveis de controlo do motor
const int stepsPerRevolution = 2048; 
Stepper myStepper(stepsPerRevolution,3,11,7,12);

//Pinos associados aos LEDs
int ledPin1 = 6;
int ledPin2 = 5;
int ledPin3 = 4;
int ledPin4 = 2;

//Pinos associados aos Botões
int bot_1Pin = 8;
int bot_2Pin = 9;
int bot_3Pin = 10;
int bot_4Pin = 0;

//Pino associado ao Speaker
int speakerPin = 13;

int bot_1 = 0;
int bot_2 = 0;
int bot_3 = 0;
int bot_4 = 0;


int length_1 = 26;
char notas_1[] = "eeeeeeegcde fffffeeeeddedg";
int beats_1[] = { 1, 1, 2, 1, 1, 2, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2};
int tempo_1 = 200;
char musica_1[] = "  FE   Sonhos   ";

int length_2 = 71;
char notas_2[] = "cfffgagavCavafggfcfffgagavCavafggffaCDCvagfgavCcfagfccfffgagavCavafggf "; // a space represents a rest
int beats_2[] = { 2,2,2,2,2,1,1,1,1,2,2,2,1,1,2,2,6,2,2,2,2,2,1,1,1,1,2,2,2,1,1,2,2,6,1,1,3,1,1,1,1,1,1,1,1,1,2,2,2,2,2,2,4,4,2,2,2,2,1,1,1,1,2,2,2,1,1,2,2,6,2};
int tempo_2 = 200;
char musica_2[] = "    CARIDADE    ";

int length_3 = 77;
char notas_3[] = "cffefaagCCCvagacffefaagCffeedcCCCfvaagCCCfvaagDDDCvavgavCffgaDDDCvavgavCffgf "; // a space represents a rest
int beats_3[] = { 2,2,3,1,2,2,2,2,2,2,3,1,2,2,4,2,2,3,1,2,2,2,2,2,2,3,1,2,2,4,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,4,2,1,1,3,1,2,2,4,3,1,2,2,2,2,4,2,1,1,3,1,2,2,4,8};
int tempo_3 = 200;
char musica_3[] = "    GRATIDAO    ";

/*
int length_1 = 4;
char notas_1[] = "eee ";
int beats_1[] = { 1, 1, 2, 1};
int tempo_1 = 200;
char musica_1[] = "    Musica 1    ";

int length_2 = 4;
char notas_2[] = "abc "; // a space represents a rest
int beats_2[] = { 2,2,2,2};
int tempo_2 = 200;
char musica_2[] = "    Musica 2    ";

int length_3 = 4;
char notas_3[] = "fff "; // a space represents a rest
int beats_3[] = { 2,2,3,4};
int tempo_3 = 200;
char musica_3[] = "    Musica 3    ";
*/

boolean ledOnOff = true;

char linha_1_inicial[] = "Feliz Natal";
char linha_2_inicial[] = "Bom Ano 2023";

void playTone(int tone, int duration) {
  for (long i = 0; i < duration * 1000L; i += tone * 2) {
    digitalWrite(speakerPin, HIGH);
    delayMicroseconds(tone);
    digitalWrite(speakerPin, LOW);
    delayMicroseconds(tone);
  }
}

void toca_notas(char note, int duration) {
  char nomes[] = {'c', 'd', 'e', 'f', 's', 'g', 'a', 'v', 'b', 'C', 'D', 'E'};
  int tons[] = { 1915, 1700, 1519, 1432, 1352, 1275, 1136, 1073, 1014, 956, 852, 758 };
  
  //toca o tom de acordo com a nota
  for (int i = 0; i < 8; i++) {
    if (nomes[i] == note) {
      playTone(tons[i], duration);
    }
  }
}

void setup() {
  pinMode(speakerPin, OUTPUT);
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);
  pinMode(ledPin4, OUTPUT);

  pinMode(bot_1Pin, INPUT_PULLUP); // push buttom 1
  pinMode(bot_2Pin, INPUT_PULLUP); // push buttom 2
  pinMode(bot_3Pin, INPUT_PULLUP); // push buttom 3
  pinMode(bot_4Pin, INPUT_PULLUP); // push buttom 4

  myStepper.setSpeed(15);

  Serial.begin(9600);//Inicialização da comunicação serial, com velocidade de comunicação de 9600

  lcd.init();                       
  lcd.backlight(); 
  lcd.setCursor(3,0); 
  lcd.print(linha_1_inicial);
  lcd.setCursor(2,1); 
  lcd.print(linha_2_inicial);

  pisca_pisca(75, 10);
  liga_as_luzes();
  roda_estrela(2);
  desliga_as_luzes();
}

void desliga_as_luzes(){
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, LOW);  
}

void liga_as_luzes(){
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, HIGH);  
}

void pisca_pisca(int velocidade, int vezes){
  for (int i=0; i<vezes; i++){
      liga_as_luzes();
      delay(velocidade);
      desliga_as_luzes();
      delay(velocidade);
  }
}

void roda_estrela(int voltas){
  myStepper.step(-stepsPerRevolution*voltas/5);
  myStepper.step(stepsPerRevolution*voltas/3);
  myStepper.step(-stepsPerRevolution*voltas/7);
  myStepper.step(stepsPerRevolution*voltas/4);
  delay(100);
}

void pisca_luzes() {
  if (ledOnOff == true) {
    digitalWrite(ledPin1, LOW);
    digitalWrite(ledPin2, HIGH);
    digitalWrite(ledPin3, LOW);
    digitalWrite(ledPin4, HIGH);

    ledOnOff = !ledOnOff;
  }
  else {
    digitalWrite(ledPin1, HIGH);
    digitalWrite(ledPin2, LOW);
    digitalWrite(ledPin3, HIGH);
    digitalWrite(ledPin4, LOW);

    ledOnOff = !ledOnOff;
  }
}

void acao_botao_1(){
    lcd.setCursor(0,1);
    lcd.print(musica_1);
    Serial.println(musica_1);
    for (int i = 0; i < length_1; i++) {
      if (notas_1[i] == ' ') {
        delay(beats_1[i] * tempo_1);
        pisca_luzes();
      } else {
        toca_notas(notas_1[i], beats_1[i] * tempo_1);
        pisca_luzes();
      }
      // pausa entre notas
      delay(tempo_1 / 2); 
    }
}

void acao_botao_2(){
    lcd.setCursor(0,1);
    lcd.print(musica_2);
    Serial.println(musica_2);   
    for (int i = 0; i < length_2; i++) {
      if (notas_2[i] == ' ') {
        delay(beats_2[i] * tempo_2);
        pisca_luzes();
      } else {
        toca_notas(notas_2[i], beats_2[i] * tempo_2);
        pisca_luzes();
      }
      delay(tempo_2 / 2); 
    }
}

void acao_botao_3(){
    lcd.print(musica_3);
    Serial.println(musica_3);
    for (int i = 0; i < length_3; i++) {
      if (notas_3[i] == ' ') {
        lcd.setCursor(0,1); // segundo numero indica linha
        delay(beats_3[i] * tempo_3);
        pisca_luzes();
      } else {
        toca_notas(notas_3[i], beats_3[i] * tempo_3);
        pisca_luzes();
      }
      delay(tempo_3 / 2); 
    }
}  
void acao_botao_4(){
    liga_as_luzes();
    roda_estrela(2);
    pisca_pisca(150,10);
}

void loop() {
  bot_1 = digitalRead(bot_1Pin);
  bot_2 = digitalRead(bot_2Pin);
  bot_3 = digitalRead(bot_3Pin);
  bot_4 = digitalRead(bot_4Pin);
  pisca_pisca(100,1);
  Serial.print(bot_1);Serial.print(bot_2);Serial.print(bot_3);Serial.println(bot_4);
  if (bot_1 == 0){
    acao_botao_1();
    }else if (bot_2 == 0){
      acao_botao_2();
      }else if (bot_3 == 0){
          acao_botao_3();
          }else if (bot_4 == 0){
              acao_botao_4();
              }    
}
