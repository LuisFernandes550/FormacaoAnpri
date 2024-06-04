#include <Ultrasonic.h>

#include <DFRobot_RGBMatrix.h> // Hardware-specific library

#define OE    9
#define LAT   10
#define CLK   11
#define A     A0
#define B     A1
#define C     A2
#define D     A3
#define E     A4
#define WIDTH 64
#define HIGH  32

#define triggerPin 6
#define echoPin 7

Ultrasonic ultrasonic(triggerPin,echoPin); //INICIALIZANDO OS PINOS DO ARDUINO
 
float distancia; //VARIÁVEL DO TIPO INTEIRO PARA DISTÂNIA
long tempo; //  TEMPO DO SINAL

DFRobot_RGBMatrix matrix(A, B, C, D, E, CLK, LAT, OE, false, WIDTH, HIGH);

const int delayTime = 50; // Ajuste a velocidade do movimento


void setup()
{
  matrix.begin();
  pinMode(echoPin, INPUT); //DEFINE O PINO COMO ENTRADA (RECEBE)
  pinMode(triggerPin, OUTPUT); //DEFINE O PINO COMO SAIDA (ENVIA)
  Serial.begin(9600);
}

void loop() {
   tempo=ultrasonic.timing();
   distancia = ultrasonic.convert(tempo, Ultrasonic::CM); //VARIÁVEL GLOBAL RECEBE O VALOR DA DISTÂNCIA ME
   Serial.print("Distância");
   Serial.println(distancia);
   delay(100);


if (distancia < 20) {
    // CONTORNO
  matrix.drawRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(7, 7, 0));
  // TEXTO BEM VINDO AO CLUBE DA ROBOTICA!
  matrix.setTextSize(1);     // size 1 == 8 pixels high
  matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves
  matrix.setCursor(3, 1);  // start at top left, with 3 pixel of spacing
  uint8_t w = 1;
  char *str = "Bem Vindo";
  for (w = 0; w < 10; w++) {
    matrix.setTextColor(Wheel(w));
    matrix.print(str[w]);
  }

  matrix.println();
  matrix.setCursor(5, 12);
  matrix.setTextColor(matrix.Color333(7, 7, 7));
  matrix.println("ao clube");

  // print each letter with a rainbow color
  matrix.setCursor(3, 23);
  matrix.setTextColor(matrix.Color333(7, 0, 0));
  matrix.print('R');
  matrix.setTextColor(matrix.Color333(7, 4, 0));
  matrix.print('O');
  matrix.setTextColor(matrix.Color333(7, 8, 0));
  matrix.print('B');
  matrix.setTextColor(matrix.Color333(4, 7, 0));
  matrix.print('0');
  matrix.setTextColor(matrix.Color333(3, 7, 0));
  matrix.print('T');
  matrix.setTextColor(matrix.Color333(0, 7, 7));
  matrix.print("I");
  matrix.setTextColor(matrix.Color333(0, 4, 7));
  matrix.print('C');
  matrix.setTextColor(matrix.Color333(0, 0, 7));
  matrix.print('A');
  matrix.setTextColor(matrix.Color333(4, 0, 7));
  matrix.print("!");
  matrix.setTextColor(matrix.Color333(7, 0, 4));
  matrix.println("!");
  delay(5000);

  // APAGAR TUDO DO ECRÃ
  matrix.fillScreen(matrix.Color333(0, 0, 0));
// CONTORNO
  matrix.drawRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(7, 0, 7));

// TEXTO AGRUPAMENTO AEFM 2023/2024
  matrix.println();
  matrix.setCursor(1, 1);
  matrix.setTextColor(matrix.Color333(7, 4, 0));
  matrix.println("Agrupament");

  matrix.println();
  matrix.setCursor(20, 12);
  matrix.setTextColor(matrix.Color333(7, 7, 7));
  matrix.println("AEFM");
  
    matrix.setCursor(6, 23);
  matrix.setTextColor(matrix.Color333(7, 0, 0));
  matrix.print('2');
  matrix.setTextColor(matrix.Color333(7, 4, 0));
  matrix.print('0');
  matrix.setTextColor(matrix.Color333(7, 8, 0));
  matrix.print('2');
  matrix.setTextColor(matrix.Color333(4, 7, 0));
  matrix.print('3');
  matrix.setTextColor(matrix.Color333(3, 7, 0));
  matrix.print('/');
  matrix.setTextColor(matrix.Color333(0, 7, 7));
  matrix.print("2");
  matrix.setTextColor(matrix.Color333(0, 4, 7));
  matrix.print('0');
  matrix.setTextColor(matrix.Color333(0, 0, 7));
  matrix.print('2');
  matrix.setTextColor(matrix.Color333(4, 0, 7));
  matrix.print("4");
  matrix.setTextColor(matrix.Color333(7, 0, 4));
  matrix.println(" ");
  delay(5000);

// APAGAR TUDO DO ECRÃ
  matrix.fillScreen(matrix.Color333(0, 0, 0));
// TEXTO o teu esforço vale a pena
  matrix.setCursor(1, 0);
  matrix.setTextColor(matrix.Color333(7, 4, 0));
  matrix.println("O teu ");

  matrix.setCursor(11, 7);
  matrix.setTextColor(matrix.Color333(4, 0, 7));
  matrix.println("esforco");

  matrix.setCursor(20, 15);
  matrix.setTextColor(matrix.Color333(0, 7, 7));
  matrix.println("vale a");
  
  matrix.setCursor(28, 23);
  matrix.setTextColor(matrix.Color333(7, 0, 0));
  matrix.print('P');
  matrix.setTextColor(matrix.Color333(7, 4, 0));
  matrix.print('E');
  matrix.setTextColor(matrix.Color333(7, 8, 0));
  matrix.print('N');
  matrix.setTextColor(matrix.Color333(4, 7, 0));
  matrix.print('A');
  matrix.setTextColor(matrix.Color333(7, 0, 4));
  matrix.println("!");
  delay(5000);
  // APAGAR TUDO DO ECRÃ
  matrix.fillScreen(matrix.Color333(0, 0, 0));
// CONTORNO
  matrix.drawRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(7, 0, 7));
  // TEXTO parabens pelo teu desempenho
  matrix.setCursor(1, 1);
  matrix.setTextColor(matrix.Color333(7, 4, 0));
  matrix.println("Parabens ");
  matrix.setCursor(11, 8);
  matrix.setTextColor(matrix.Color333(4, 0, 7));
  matrix.println("pelo");
  matrix.setCursor(30, 16);
  matrix.setTextColor(matrix.Color333(0, 7, 7));
  matrix.println("teu");
  matrix.setCursor(3, 24);
  matrix.setTextColor(matrix.Color333(3, 7, 0));
  matrix.println("DESEMPENHO");

  delay(5000);
  // APAGAR TUDO DO ECRÃ
  matrix.fillScreen(matrix.Color333(0, 0, 0));

  // TEXTO COM ATITUDE O SUCESSO É GARANTIDO
   matrix.setCursor(1, 1);
  matrix.setTextColor(matrix.Color333(7, 4, 0));
  matrix.println("Com ");
  matrix.setCursor(8, 8);
  matrix.setTextColor(matrix.Color333(4, 0, 7));
  matrix.println("atitude o");
  matrix.setCursor(7, 16);
  matrix.setTextColor(matrix.Color333(0, 7, 7));
  matrix.println("sucesso e");
  matrix.setCursor(3, 24);
  matrix.setTextColor(matrix.Color333(3, 7, 0));
  matrix.println("GARANTIDO");
  delay(5000);

// APAGAR TUDO DO ECRÃ
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  } else {
    matrix.fillScreen(matrix.Color333(0, 0, 0)); // Desliga a matriz
  
  }

 
}


// Input a value 0 to 24 to get a color value.
// The colours are a transition r - g - b - back to r.
uint16_t Wheel(byte WheelPos) {
  if (WheelPos < 8) {
    return matrix.Color333(7 - WheelPos, WheelPos, 0);
  } else if (WheelPos < 16) {
    WheelPos -= 8;
    return matrix.Color333(0, 7 - WheelPos, WheelPos);
  } else {
    WheelPos -= 16;
    return matrix.Color333(0, WheelPos, 7 - WheelPos);
  }
}
