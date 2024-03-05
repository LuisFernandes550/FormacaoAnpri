
//  ---  STOP - LED Game  ---
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Inicializar o display 16x2 no endereço 0x27
const int btnPin = 2;
const int buzzerPin = A0;
const int ledPin[] = { 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 };
int i, j;
int tempo = 600; //Tempo de acionamento do LED
int nivel = 0; // Variável para armazenar o nível do jogador

void setup() {
  //Serial.begin(9600); //Inicialização da comunicação serial
  lcd.init();       // inicializar o display LCD I2C
  pinMode(btnPin, INPUT); //Define o pino digital 13 (botão) como entrada
  pinMode(buzzerPin, OUTPUT); //Define o pino analógico A5 (buzzer) como saída
  for (i = 0; i < 11; i++) {
    pinMode (ledPin[i], OUTPUT);//Configura os pinos em que os LEDs estão ligados como saída
  }
  //Serial.print("Nivel -> "); //Imprime na serial "Nivel -> "
  //Serial.println(nivel); //Imprime na serial o valor de nível
  lcd.backlight();  // Acender a luz de fundo do LCD
  lcd.setCursor(0, 0);      // Colocar o cursor na primeira coluna da primeira linha
  lcd.print("   ** JOGO **");  // Escrever o texto "** JOGO **"
  lcd.setCursor(0, 1);      // Colocar o cursor na primeira coluna da primeira linha
  lcd.print(" STOP-Led Game");  // Escrever o texto "STOP-Led Game"
  delay (5000);
  lcd.clear();
  lcd.setCursor(0, 0);      // Colocar o cursor na primeira coluna da primeira linha
  lcd.print("Nivel -> ");  // Escrever o texto "Nivel ->"
  lcd.setCursor(9, 0);
  lcd.print (nivel); // Escrever o valor da variável relativo ao nível
}

void loop() {
  for (i = 0; i < 11; i++) {
    digitalWrite(ledPin[i], HIGH);
    delay(tempo); //Intervalo de 600 milissegundos (tempo)

    if (digitalRead(btnPin) == 1) { //Se a leitura do pino 13 (botão) for igual a 1
      while (digitalRead(btnPin)); //Enquanto o botão estiver pressionado o jogo fica parado

      if (ledPin[i] == 8) { //Se o led conetado no pino 7, led vermelho,estiver aceso (pino do led vermelho)
        buzzerRed();
        nivel = nivel + 1; //Acrescenta nivel em 1
        //Serial.print("Nivel -> "); //Imprime na serial "Nivel -> "
        //Serial.println(nivel); //Imprime na serial o valor de nível
        lcd.setCursor(0, 0);      // Colocar o cursor na primeira coluna da primeira linha
        lcd.print("Nivel ->      ");  // Escrever o texto "Nivel->"
        lcd.setCursor(9, 0);
        lcd.print (nivel); // Escrever o valor da variável relativo ao nível
        tempo = tempo - 100; //Diminui tempo em 100 milissegundos

        if (tempo == 0) {
          lcd.setCursor(0, 1);
          lcd.print ("Parabens!!!"); // Escrever o valor da variável relativo ao nível

        }
      }
      else { //Senão
        buzzerOver();
        //Serial.println("GAME OVER"); //Imprime na serial "GAME OVER"
        lcd.clear();
        lcd.setCursor(0, 0);      // Colocar o cursor na primeira coluna da primeira linha
        lcd.print(" !!GAME OVER!!");

        game_over(); //Chama a função game_over()
        tempo = 600; //Retorna tempo = 600
        i = 0; //Retorna i=0 (led do pino 2)
        nivel = 0; //Retorna nivel = 0

        //Serial.print("Nivel -> "); //Imprime na serial "Nivel -> "
        //Serial.println(nivel); //Imprime na serial o valor de nível
        lcd.setCursor(0, 0);      // Colocar o cursor na primeira coluna da primeira linha
        lcd.print("Nivel ->      ");  // Escrever o texto "Nivel"
        lcd.setCursor(9, 0);
        lcd.print (nivel); // Escrever o valor da variável relativo ao nível
      }
    }
    digitalWrite(ledPin[i], LOW);
  }
  for (i = 10; i >= 0; i--) {
    digitalWrite(ledPin[i], HIGH);
    delay(tempo); //Intervalo de 600 milissegundos (tempo)
    digitalWrite(ledPin[i], LOW);

    if (digitalRead(btnPin) == 1) { //Se a leitura do pino 13 (botão) for igual a 1
      while (digitalRead(btnPin)); //Enquanto o botão estiver pressionado o jogo fica parado

      if (ledPin[i] == 8) { //Se led vermelho estiver aceso (pino do led vermelho)
        buzzerRed();
        nivel = nivel + 1; //Acrescenta nivel em 1

        //Serial.print("Nivel -> "); //Imprime na serial "Nivel -> "
        //Serial.println(nivel); //Imprime na serial o valor de nível
        lcd.setCursor(0, 0);      // Colocar o cursor na primeira coluna da primeira linha
        lcd.print("Nivel ->      ");  // Escrever o texto "Nivel"
        lcd.setCursor(9, 0);
        lcd.print (nivel); // Escrever o valor da variável relativo ao nível
        tempo = tempo - 100; //Diminui tempo em 100 milissegundos

        if (tempo == 0) {
          lcd.setCursor(0, 1);
          lcd.print ("Parabens!!!"); // Escrever o valor da variável relativo ao nível

        }
      }
      else { //Senão
        buzzerOver();
        //Serial.println("GAME OVER"); //Imprime na serial "GAME OVER"
        lcd.clear();
        lcd.setCursor(0, 0);      // Colocar o cursor na primeira coluna da primeira linha
        lcd.print(" !!GAME OVER!!");

        game_over(); //Imprime na serial "GAME OVER"
        tempo = 600; //Retorna tempo = 600
        i = 10; //Retorna i = 10
        nivel = 0; //Retorna nível = 0
        //Serial.print("Nivel -> "); //Imprime na serial "Nivel -> "
        //Serial.println(nivel); //Imprime na serial o valor de nível
        lcd.setCursor(0, 0);      // Colocar o cursor na primeira coluna da primeira linha
        lcd.print("Nivel ->      ");  // Escrever o texto "Nivel"
        lcd.setCursor(9, 0);
        lcd.print (nivel); // Escrever o valor da variável relativo ao nível
      }
    }
    digitalWrite(ledPin[i], LOW);
  }
  
}

//Função buzzer()
void buzzerRed() {
  tone(buzzerPin, 50);
  delay(100);
  noTone(buzzerPin);
}
void buzzerOver() {
  tone(buzzerPin, 297);
  delay(300);
  tone(buzzerPin, 352);
  delay(200);
  tone(buzzerPin, 264);
  delay(100);
  noTone(buzzerPin);
}

//Função game_over()
void game_over() {
  for (j = 0; j < 3; j++) //Inicializa j = 0, enquanto j for menor que 4, decrementa j em 1
    for (i = 0; i < 11; i++) {
      digitalWrite(ledPin[i], HIGH);
      digitalWrite(ledPin[10] - ledPin[i], HIGH);
      delay(100); //Intervalo de 100 ms
      digitalWrite(ledPin[i], LOW);
      digitalWrite(ledPin[10] - ledPin[i], LOW);
    }
}
