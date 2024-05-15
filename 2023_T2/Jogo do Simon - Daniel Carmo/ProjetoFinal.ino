#define NOTA_1  230
#define NOTA_2  270
#define NOTA_3  310
#define NOTA_4  350
#define ledRed	11
#define ledBlue	10
#define ledGreen	9
#define ledYellow	6
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
#include <time.h>

int duracao = 500;
int intervalo = 100;
int pausa = 0;
  
int buttonRed=0;
int buttonBlue=0;
int buttonGreen=0;
int buttonYellow=0;
int buttonBlack=0; //botão da dificuldade

int lido = 0;
int numClicks = 0; //variável que irá conter o número de vezes que pressionamos um botão
int numTons = 0; //variável que irá conter o número de tons/leds reproduzidos
int nivel = 1; //variável que determina quantos tons/leds irão ser reproduzidos (em sequência)
int dificuldade = 0; ////variável que irá conter o nível de dificuldade

int notas[4] = {NOTA_1, NOTA_2, NOTA_3, NOTA_4};
int desafio[10] = {}; //array que vai conter as notas que ião ser necessárias descobrir


void desligaLeds(){ //apaga os LED's
    digitalWrite(ledYellow,LOW);
  	digitalWrite(ledRed,LOW);
  	digitalWrite(ledBlue,LOW);
  	digitalWrite(ledGreen,LOW);
}

void intro(){//Função para tocar a sequência de notas (ou tons) existentes e acender os leds respetivos - Introdução
    for (int contador = 0; contador < 4; contador++) 
  {
    switch (contador) {
      case 0:
      	digitalWrite(ledRed,HIGH);
        break;
      case 1:
      	digitalWrite(ledBlue,HIGH);
        break;
      case 2:
      	digitalWrite(ledGreen,HIGH);
        break;
      case 3:
      	digitalWrite(ledYellow,HIGH);
        break;
      }

    tone(8, notas[contador], duracao); //descomentar para ouvir o som
    pausa = duracao * 0.5;
    delay(pausa); 
    
    desligaLeds();
	  noTone(8);
    }
    
    lcd.setCursor(0, 0);
  	lcd.print("Jogo Memoria");
  	lcd.setCursor(0, 1);
  	lcd.print("Vamos comecar...");
    delay(2000);
    lcd.setCursor(0, 1);
    lcd.print("Pronto??...          ");
    delay(2000);
    lcd.setCursor(0, 1);
  	lcd.print("                     ");
  	lcd.setBacklight(1);
	  delay(1000);
}

void lcdAcerto(int numero){ //Mostra no LCD o progresso de acertos e mensagens de acertos
	lcd.setCursor(0, 1);

   switch (numero) {
      case 0:
      	lcd.print("                  "); //limpar linha
        break;
      case 1:
      	lcd.print("X");
        break;
      case 2:
      	lcd.print("X X");
        break;
      case 3:
      	lcd.print("X X X");
        break;
      case 4:
      	lcd.print("X X X X");
        break;
     case 5:
      	lcd.print("X X X X X");
        break;
     case 6:
      	lcd.print("X X X X X X");
        break;
     case 7:
      	lcd.print("X X X X X X X");
        break;
     case 8:
      	lcd.print("X X X X X X X X");
        break;
	 case 9: //terminou sequência com sucesso
      	lcd.setCursor(0, 0);
        lcd.print("Boa! Acertaste!!      ");
        lcd.setCursor(0, 1);
        lcd.print("Nivel seguinte...     ");
        delay(2000);
        lcd.setCursor(0, 0);
        lcd.print("Jogo Memoria        ");
        lcd.setCursor(0, 1);
        lcd.print("                   ");
        break;
	case 10: //acertou na sequência mais longa ("VENCE" O JOGO)
      	lcd.setCursor(0, 0); 
        lcd.print("PARABENS!           ");
        lcd.setCursor(0, 1);
        lcd.print("GANHASTE!!       ");
        delay(3000);
        break;
      }
}

void lcdErro(){ //função para mostras as mensagens de erro na sequência e informar o utilizador do reinicio do jogo
  lcd.setCursor(0, 0);
  lcd.print("Oh! Falhaste!!     ");
  delay(1500);
  lcd.setCursor(0, 0);
  lcd.print("Vamos tentar        ");
  lcd.setCursor(0, 1);
  lcd.print("de novo!?            ");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("Estas pronto?!      ");
  lcd.setCursor(0, 1);
  lcd.print("Vamos a isto!       ");
  delay(2000);
  lcd.setCursor(0, 0);
  lcd.print("Jogo Memoria        ");
  lcd.setCursor(0, 1);
  lcd.print("Escuta e repete...            ");
}

void mostraDesafio(){ //Função que mostra o conteúdo do array no Serial Monitor (testes)
  for (int contador = 0; contador < 8; contador++) 
  {
    Serial.print("[");
    Serial.print(contador);
    Serial.print("]");Serial.print("=>");    
    Serial.println(desafio[contador]);
  }
}

void criadesafio(){ //Função para criar o array com "notas" aleatórias
  randomSeed(analogRead(A0));
  for (int contador = 0; contador < 8; contador++) 
  {
    desafio[contador]=(random(0, 4));
  }
  mostraDesafio();
}

void  aumentaDificuldade(){ //Função que irá diminuir o tempo de visualização/audição da nota
 if (dificuldade==0){
    digitalWrite(ledRed,HIGH);
   	delay(250);
    digitalWrite(ledRed,LOW);
    lcd.setCursor(0, 1);
    lcd.print("#Dificuldade 1#");
    delay(2000);
 }
 if (dificuldade==1){
    digitalWrite(ledRed,HIGH);
    digitalWrite(ledYellow,HIGH);
   	delay(250);
    digitalWrite(ledRed,LOW);
    digitalWrite(ledYellow,LOW);
    lcd.setCursor(0, 1);
    lcd.print("#Dificuldade 2#");
    delay(2000);
 }
  if (dificuldade==2){
    digitalWrite(ledRed,HIGH);
    digitalWrite(ledYellow,HIGH);
    digitalWrite(ledGreen,HIGH);
   	delay(250);
    digitalWrite(ledRed,LOW);
    digitalWrite(ledYellow,LOW);
    digitalWrite(ledGreen,LOW);
    lcd.setCursor(0, 1);
    lcd.print("#Dificuldade 3#");
    delay(2000);
 }
  if (dificuldade==3){
    digitalWrite(ledRed,HIGH);
    digitalWrite(ledYellow,HIGH);
    digitalWrite(ledGreen,HIGH);
    digitalWrite(ledBlue,HIGH);
   	delay(250);
    digitalWrite(ledRed,LOW);
    digitalWrite(ledYellow,LOW);
    digitalWrite(ledGreen,LOW);
    digitalWrite(ledBlue,LOW);
    lcd.setCursor(0, 1);
    lcd.print("#Dificuldade 4#");
    delay(2000);
 }

 if(dificuldade<4){
  dificuldade++;
  duracao = duracao-100; 
 }else{
  dificuldade=0;
  duracao = 500;
  lcd.setCursor(0, 1);
  lcd.print("#Dificuldade 0#");
  delay(2000);
 }

  criadesafio();
  intro();
  Serial.println("######################################");
  numClicks=0; 
  lcdAcerto(0); //limpa o "contador" de "acertos"
  numTons=0;
  nivel=1;
  led_nota(desafio[0]);//toca a primeira nota do desafio
}

void led_nota(int nota){//Função para tocar as notas e acender os leds respetivos
    switch (nota) {
      case 0:
      	digitalWrite(ledRed,HIGH);
        break;
      case 1:
      	digitalWrite(ledBlue,HIGH);
        break;
      case 2:
      	digitalWrite(ledGreen,HIGH);
        break;
      case 3:
      	digitalWrite(ledYellow,HIGH);
        break;
      }
  
    tone(8, notas[nota], duracao); //descomentar para ouvir o som
    pausa = duracao * 0.5;
    delay(pausa); 
    noTone(8);
    //delay(intervalo);
    desligaLeds(); //Apagar os LEDs
}

void tocaErro(){ //Toca o som do erro e desliga os leds
	  tone(8, 150 , duracao*4);
  	  delay(200); 
  	  noTone(8);
      desligaLeds();
  	  delay(500);
}

void tocaCerto(int som){ //Toca o som da nota -> acertada
	  tone(8, som , duracao);
  	  //int pausa = duracao * 0.2; 
      delay(250); 
  	  noTone(8);
  	  desligaLeds();
      delay(200);
}

void verificaLido(int botao){ //Função "principal" para verificação das sequências de "Clicks"
  if ((botao!=desafio[numTons]))//falhou sequência - RECOMEÇAR!!!
   	{
	  	tocaErro(); //toca o som de erro
    	lcdErro(); // mostra as mensagens de erro no LCD
    	
    	numClicks=0; 
    	lcdAcerto(0); //limpa o "contador" de "acertos"
    	led_nota(desafio[0]);
    	numTons=0;
    	nivel=1;
    }
    else//acertou!! continuar...
    {
      tocaCerto(notas[botao]); //chama a função 'tocaCerto' que toca o "tom"/"nota" (no array 'notas') associado ao botão (index) pressionado 
      
      numTons++;
      lcdAcerto(numTons);
      if ((numClicks==8)&&(nivel==8)) //FINAL DO JOGO - ACERTO NA SEQUENCIA COMPLETA
  		{
    	  //Serial.println("GANHOU O JOGO!!"); //TESTES
		    lcdAcerto(10);//mostra mensagem de conclusão e fim do jogo
       	}
  	  else //Acertou num dos "tons"/"led" da sequência 
  		{
          if((numClicks==numTons)&&(numClicks==nivel)) //Se o número de "clicks" coincide com o número de "tons"/"leds" e é também igual ao "nível" que se encontra então quer dizer que terminou a sequencia intermédia
          {
            nivel++; //aumentar o nível
            lcdAcerto(9);//mostra mensagem fim de sequencia
            	
            	for (int i=0; i<numTons+1; i++) //reproduz os "tons"/"leds" do próximo nível
      			{
        			led_nota(desafio[i]);
              delay(300); //??
      			}
            	numClicks=0; //reiniciar o número de "clicks" e "tons"
            	numTons=0;
          }
          else //ainda não chegámos ao final da sequencia
          {
            // aqui não acontece nada!
          }
        }
    }
}

void setup()   /******** SETUP ***********/
{
  lcd.init(); //Inicializar LCD
  lcd.setBacklight(HIGH); 
  Serial.begin(9600);
  //LEDs
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(6,OUTPUT);
  //Botões
  pinMode(5,INPUT);
  pinMode(4,INPUT);
  pinMode(3,INPUT);
  pinMode(2,INPUT);
  pinMode(7,INPUT);

  intro();//toca as notas e acende os leds
  criadesafio();//criar o array com notas aleatórias
  led_nota(desafio[0]);//toca a primeira nota do desafio
  
}

void loop() { /******** LOOP ***********/
  buttonRed = digitalRead(2);
  buttonBlue = digitalRead(3);
  buttonGreen = digitalRead(4);
  buttonYellow = digitalRead(5);
  buttonBlack = digitalRead(7);
  
  if (buttonRed == HIGH) {
    digitalWrite(ledRed,HIGH);
    numClicks++;
    verificaLido(0);
   	delay(250);
    digitalWrite(ledRed,LOW);
	  }
  
  if (buttonBlue == HIGH) {
    digitalWrite(ledBlue,HIGH);
    numClicks++;
	  verificaLido(1);
    delay(250);
    digitalWrite(ledBlue,LOW);
  }

  if (buttonGreen == HIGH) {
    digitalWrite(ledGreen,HIGH);
    numClicks++;
	  verificaLido(2);
    delay(250);
    digitalWrite(ledGreen,LOW);
  }
  
  if (buttonYellow == HIGH) {
    digitalWrite(ledYellow,HIGH);
    numClicks++;
    verificaLido(3);
    delay(250);
    digitalWrite(ledYellow,LOW);
  }
 	
    if (buttonBlack == HIGH) {
      aumentaDificuldade();
  }
}