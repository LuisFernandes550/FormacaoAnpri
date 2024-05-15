// =====================================================================
//            
// led pino 7 = verde (50 pontos) - currentLED=4
// led pino 6 e led pino 8 = amarelo (20 pontos) - currentLED = 3 e 5
// led pino 5 e led pino 9 = laranja (10 pontos) - currentLED = 2 e 6
// leds = vermelho (0 pontos)
//
// =====================================================================   


#include <LCD.h>
#include <LiquidCrystal.h>
#include <LiquidCrystal_I2C.h>
#include <SI2CIO.h>
      

// biblioteca LCS com I2C
#include <LiquidCrystal_I2C.h>

// Inicializa o display no endereco 0x27
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7,3, POSITIVE);

// hardware
byte ledPin[] = {3,4,5,6,7,8,9,10,11}; // leds
byte butPin = 2; // push button

// variáveis
int shot = 0;
boolean gameStatus = false;
boolean gameOverStatus = false;
byte currentLED = 0;
int dirFlag = 1;
int points = 0;
int record = 0;

void setup(){
  // define hardware - entradas e saídas
  for (byte i = 0; i<9; i++) {
    pinMode(ledPin[i], OUTPUT);
  }
  pinMode(butPin, INPUT_PULLUP);
  // inicia monitor serial e display LCD
  Serial.begin(9600);
  lcd.begin (16,2);
  // inicia interrupsão externa
  attachInterrupt(digitalPinToInterrupt(butPin), buttonClick, FALLING);
  

  showStart(); // exibe display para iniciar jogo
  allLedsOn(); // acende todos os leds 
  Serial.println("================================");
  Serial.println("Novo Jogo - Clique para iniciar");      
} // end setup

void loop() {
 if (!gameStatus) {
    if (gameOverStatus == true) {
      gameOver();
    } else if (gameOverStatus == false)  {
      allLedsOff();
      checkButton();
    }
 } else if (gameStatus){     
  moveLed();   
 }

} //end loop



void moveLed() {    
    showGame(); // exibe pontos no lcd    
    digitalWrite(ledPin[currentLED], HIGH); // acende o LED atual
    if (shot >0 && shot<6) delay(100/(6-shot));  // dificuldade do jogo     
    currentLED += dirFlag; // incrementa de acordo com o valor     
    allLedsOff();
    // altera a direção se tivermos atingido o fim
    if (currentLED == 9) dirFlag = -1;
    if (currentLED == 0) dirFlag = 1;

    if (shot<=0) { 
      gameStatus = false;
      gameOverStatus = true;     
   }
}

// ao clicar o botão durante o jogo
void buttonClick() {      
    // calcula pontuação
    if (currentLED == 4) points += 50;
    if (currentLED == 3 || currentLED == 5) points += 20;
    if (currentLED == 2 || currentLED == 6) points += 10;
    showGameMonitor(); // exibe pontos no monitor 
    // calcula jogadas     
    if (gameStatus == true) {
      shot--;             
  }
  delayMicroseconds(50000); 
}

void showGameMonitor() {
  if (shot > 0) {
  Serial.print(currentLED); // exibe o led que foi clicado
  if (currentLED == 4) {
      Serial.println(" | Acertou o Led verde -> 50 pontos - SHOT!!!");
    } else if (currentLED == 3 || currentLED == 5){
      Serial.println(" | Acertou o led laranja -> 20 pontos");
    } else if (currentLED == 2 || currentLED == 6){
      Serial.println(" | Acertou o led amarelo -> 10 pontos");
    } else Serial.println(" | Acertou o led vermelho -> 0 pontos");
  }  
}

  // exibe no lcd
void showGame(){
  // pontos
  lcd.setCursor(0,0);
  lcd.print("Pontos: ");
  lcd.setCursor(9,0);
  lcd.print(points);
  
  // shot
  lcd.setCursor(0,1);
  lcd.print("Shot: ");
  lcd.setCursor(6,1);
  lcd.print(shot);

  // record
  lcd.setCursor(9,1);
  lcd.print("R= ");
  lcd.setCursor(12,1);
  lcd.print(record);
}



// clique para iniciar jogo
void showStart() {
  for (byte i=0;i<3;i++) {
    lcd.setBacklight(HIGH);
    lcd.setCursor(2,0);
    lcd.print("CLIQUE PARA");
    lcd.setCursor(1,1);
    lcd.print("INICIAR O JOGO!");
    delay(300);
    lcd.setBacklight(LOW);
    delay(300);
  }
   lcd.setBacklight(HIGH);
}

// acende todos os leds
void allLedsOn() {
  for (byte i = 0; i<9; i++) {
    digitalWrite(ledPin[i], HIGH);    
  }
  delay(1000);
}

// apaga todos os leds
void allLedsOff() {
   for (byte i = 0; i<9; i++) {
    digitalWrite(ledPin[i], LOW);  
  }
}

// verifica se o botão foi acionado
void checkButton() {
  if (!digitalRead(butPin) && gameStatus == false) {
    if(shot == 0) {
      shot = 5;
      gameStatus = true;
      lcd.clear(); 
    }
    while(!digitalRead(butPin)) {}
    delay(50);
  }
}

//================ GAME OVER =================
void gameOver() {
 // imprime no monitor serial
 Serial.print("Game Over | ");
 Serial.print("Seus Pontos: "); 
 Serial.println(points);
 if (points > record) Serial.println(" *** NOVO RECORDE *** ");
 Serial.println("================================");
 Serial.println("Novo Jogo - Clique para iniciar");
 // imprime no display
 lcd.clear();
 lcd.setCursor(3,0);
 lcd.print("GAME OVER");
 lcd.setCursor(0,1);
 lcd.print("SEUS PONTOS: ");
 lcd.setCursor(12,1);
 lcd.print(points);
 delay(5000);
 lcd.clear();
 
 // seta record
 if (points > record) {
  record = points;
  recordShow();
 } else {
   
 
  showStart(); // exibe display para iniciar jogo
  allLedsOn(); // acende todos os leds 
 }


 shot = 0;
 gameStatus = false;
 gameOverStatus = false;
 currentLED = 0;
 points = 0;
 dirFlag = 1;
 
}


void recordShow() {
for (byte i=0;i<3;i++) {
    lcd.setBacklight(HIGH);
    lcd.setCursor(1,0);
    lcd.print("*NOVO RECORD*");
    lcd.setCursor(2,1);
    lcd.print(points);
    lcd.setCursor(6,1);
    lcd.print(" Pontos");
    delay(500);
    lcd.setBacklight(LOW);
    delay(500);
  }
   lcd.setBacklight(HIGH);
   delay(2000);
   lcd.clear();
   //  inicializar jogo
   lcd.setCursor(2,0);
   lcd.print("CLIQUE PARA");
   lcd.setCursor(1,1);
   lcd.print("INICIAR O JOGO!");
   allLedsOn();
   delay(500);  
}