#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int veiculoVermelho = 12, veiculoAmarelo = 11, veiculoVerde = 10; //Definição dos pinos para LEDs do veiculo
const int peoesVermelho = 9, peoesVerde = 8;                            //Definição dos pinos para LEDs de peões
const int botao = 2;                                                    //Definição do pino de input do botao
const int tmpAtravessar = 10000;                                        //tempo permitido para atravessar a passadeira
int state;                                                              //verificar estado do botão                               

unsigned long tmpToque;  //tempo desde que o botão foi pressionado

//controlo display 7 segmentos
const int segmentos[] = { 41, 43, 39, 37, 35, 31, 33 };  //Segmentos A, B, C, D, E, F, G
const String numeros[] = { "1111011", "1111111", "1110000", 
                            "1011111", "1011011", "0110011", 
                            "1111001",  "1101101", "0110000",
                             "1111110" };

char sequencia[8];

LiquidCrystal_I2C lcd(0x27, 16, 2);  // Criar LCD address to 0x27 for a 16 chars and 2 line display

const int somPin = 13;  //Definição do pin para o buzzer

//Ciclos
int i, j, x;


void setup() {
  //OutPuts
  pinMode(veiculoVermelho, OUTPUT);
  pinMode(veiculoAmarelo, OUTPUT);
  pinMode(veiculoVerde, OUTPUT);
  pinMode(peoesVermelho, OUTPUT);
  pinMode(peoesVerde, OUTPUT);

  for (i = 0; i < 7; i++) {
    pinMode(segmentos[i], OUTPUT);
  }

  pinMode(somPin, OUTPUT);

  //InPut
  pinMode(botao, INPUT);

  //Definir estado inicial
  digitalWrite(veiculoVerde, HIGH);
  digitalWrite(peoesVermelho, HIGH);

  lcd.init();  // initialize the lcd
  lcd.backlight();
  msg(0);
  //Serial.begin(9600);
}

void loop() {
  state = digitalRead(botao);
  //Serial.println(botao);
  if (state == HIGH && (millis() - tmpToque) > tmpAtravessar) {
    mudar();
  }
}

void mudar() {
  digitalWrite(veiculoVerde, LOW);
  digitalWrite(veiculoAmarelo, HIGH);
  delay(2000);
  digitalWrite(veiculoAmarelo, LOW);
  digitalWrite(veiculoVermelho, HIGH);
  delay(1000);

  msg(1);

  digitalWrite(peoesVermelho, LOW);
  digitalWrite(peoesVerde, HIGH);

  //delay(tmpAtravessar);  //Aguarda o tempo de atravessar do peão
  delay(1000);

  for (x = 0; x < 10; x++) {
    numeros[x].toCharArray(sequencia, 8);  //cria um novo array de carateres por cada posição do array numeros
    for (j = 0; j < 7; j++) {
      digitalWrite(segmentos[j], sequencia[j] - '0');  //sequencia[j]-'0' converte o carater em número
    }
    tone(somPin, 329);
    if (x > 7) {
      digitalWrite(peoesVerde, HIGH);
      delay(250);
      digitalWrite(peoesVerde, LOW);
      delay(250);
    } else {
      delay(500);
    }
    noTone(somPin);
    delay(500);
  }

  digitalWrite(peoesVermelho, HIGH);
  msg(0);
  delay(500);

  digitalWrite(veiculoVermelho, LOW);
  digitalWrite(veiculoVerde, HIGH);

  tmpToque = millis();
}


void msg(int m) {
  lcd.clear();
  if (m == 1) {
    lcd.setCursor(0, 0);
    lcd.print("Pode Atravessar");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("Nao Atravessar");
  }
}