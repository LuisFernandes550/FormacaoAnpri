//Adicionada a biblioteca "FrequencyTimer2.h"
#include <FrequencyTimer2.h>

//Matrizes para cada letra
#define B { \
    {0, 1, 1, 1, 1, 1, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 1, 1, 1, 1, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 1, 1, 1, 1, 0, 0}, \
  }

#define E { \
    {0, 1, 1, 1, 1, 1, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 1, 1, 1, 1, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 0, 0}, \
    {0, 1, 1, 1, 1, 1, 1, 0}, \
  }

#define M { \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 1, 0, 0, 1, 1, 0}, \
    {0, 1, 0, 1, 1, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0} \
  }

#define V { \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 0, 1, 0, 0, 1, 0, 0}, \
    {0, 0, 0, 1, 1, 0, 0, 0} \
  }

#define I { \
    {0, 0, 1, 1, 1, 1, 0, 0}, \
    {0, 0, 0, 1, 1, 0, 0, 0}, \
    {0, 0, 0, 1, 1, 0, 0, 0}, \
    {0, 0, 0, 1, 1, 0, 0, 0}, \
    {0, 0, 0, 1, 1, 0, 0, 0}, \
    {0, 0, 0, 1, 1, 0, 0, 0}, \
    {0, 0, 0, 1, 1, 0, 0, 0}, \
    {0, 0, 1, 1, 1, 1, 0, 0} \
  }

#define N { \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 1, 0, 0, 0, 1, 0}, \
    {0, 1, 1, 1, 0, 0, 1, 0}, \
    {0, 1, 0, 1, 1, 0, 1, 0}, \
    {0, 1, 0, 0, 1, 1, 1, 0}, \
    {0, 1, 0, 0, 0, 1, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0} \
  }

#define D { \
    {0, 1, 1, 1, 1, 0, 0, 0}, \
    {0, 1, 0, 0, 0, 1, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 1, 0, 0}, \
    {0, 1, 1, 1, 1, 0, 0, 0} \
  }

#define O { \
    {0, 0, 1, 1, 1, 1, 0, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 1, 0, 0, 0, 0, 1, 0}, \
    {0, 0, 1, 1, 1, 1, 0, 0} \
  }

//Espaço
#define Z { \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0}, \
    {0, 0, 0, 0, 0, 0, 0, 0} \
  }

byte col = 0;

//Matriz de leds 8x8
byte leds[8][8];

int portas[17] = { 0, 5, 4, 3, 2, 14, 15, 16, 17, 13, 12, 11, 10, 9, 8, 7, 6 };


int colunas[8] = {9, 3, 2, 12, 15, 11, 7, 6};


int linhas[8] = {13, 8, 17, 10, 5, 16, 4, 14};


const int transicoes = 9;

//Imprimindo no painel
byte padroes[transicoes][8][8] = {B, E, M, V, I, N, D, O, Z};

int padrao = 0;

void setup() {

//Porta/pino do potenciómetro
pinMode(A4, INPUT);


for (int i = 1; i <= 16; i++) {
  pinMode(portas[i], OUTPUT);
}

//Definindo rotina de interrupção a ser chamada
FrequencyTimer2::setOnOverflow(display);

preencherEcra(padrao);
}

void loop() {

  padrao = ++padrao % transicoes;

  mostrarCarater(padrao, analogRead(A4));
}


void preencherEcra(int padrao) {

//Percorre as linhas 'i' e as colunas 'j' uma por uma
for (int i = 0; i < 8; i++) {
  for (int j = 0; j < 8; j++) {
    leds[i][j] = padroes[padrao][i][j];
    }
  }
}

void mostrarCarater(int padrao, const int valorPot) {

//Deslocar os carateres da direita para a esquerda
  for (int l = 0; l < 8; l++) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {

        leds[j][i] = leds[j][i + 1];
      }
    }

    for (int j = 0; j < 8; j++) {
      leds[j][7] = padroes[padrao][j][0 + l];
    }

    delay(valorPot);
  }
}


void display() {

  digitalWrite(colunas[col], HIGH);
  col++;
  if (col == 8) {
    col = 0;
  }
  for (int linha = 0; linha < 8; linha++) {
    if (leds[col][7 - linha] == 1) {


      digitalWrite(linhas[linha], HIGH);
    }
    else {

      digitalWrite(linhas[linha], LOW);
    }
  }

  digitalWrite(colunas[col], LOW);
  delay(analogRead(A4));
}
