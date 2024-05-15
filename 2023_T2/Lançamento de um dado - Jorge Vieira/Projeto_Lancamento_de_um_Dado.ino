// C++ code
#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>
#include <SoftwareSerial.h>

//Definição das portas
const int botPin = A3;
const int echoPin = 4;
const int triggerPin = 5;
const int buzzerPin = 6;
const int ledPin1 = 7;
const int ledPin2 = 8;
const int ledPin3 = 9;
const int ledPin4 = 10;
const int ledPin5 = 11;
const int ledPin6 = 12;
const int ledPin7 = 13;

// Configuração da Animação dos Led's
const int listaAnim[] = {7, 8, 9, 10, 11, 12, 13, 10, 7, 8, 9, 10,
                                                        11, 12, 13};
const int compLista = 15;

// Tons para o buzzer
const int tones[] = {262, 294, 330, 349};

// Inicialização do LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Inicialização do sensor Ultrasónico
Ultrasonic ultrasonic(triggerPin, echoPin);

// Inicialização do módulo blutooth HC-05
SoftwareSerial mySerial(2, 3); // RX, TX

long tempo;
float dist;

int randNum;
int botao;

void setup()
{
 	//Prepara para a geração de numeros aleatorios.
	randomSeed(analogRead(A0));
  
	//Configuração das portas
	pinMode(ledPin1, OUTPUT);
	pinMode(ledPin2, OUTPUT);
	pinMode(ledPin3, OUTPUT);
	pinMode(ledPin4, OUTPUT);
	pinMode(ledPin5, OUTPUT);
	pinMode(ledPin6, OUTPUT);
	pinMode(ledPin7, OUTPUT);
	pinMode(botPin, INPUT);  	
	pinMode(buzzerPin, OUTPUT);
    
  // initialização do LCD
	lcd.init();
	lcd.backlight();

	lcd.setCursor(0, 0);
	lcd.print("   Lancamento  ");
	lcd.setCursor(0, 1);
	lcd.print("   de um DADO  ");
    
	//inicia com os leds desligados
	desligaLeds();
	
	//Serial.begin(9600);
  mySerial.begin(9600);
}

void loop()
{
  // Lê a distância no sensor Utrasónico
	tempo = ultrasonic.timing();
	dist = ultrasonic.convert(tempo, Ultrasonic::CM);

	//Serial.print("Distancia: ");
	//Serial.println(dist);
	delay(100);

  // Lê informação a partir do módulo bluetooth
  int appRead = 0;
  if(mySerial.available())
  {
    appRead = mySerial.readString().toInt();
    Serial.println(appRead);
  }

  // Lê dados do Botão de pressão
  botao = analogRead(botPin);

  // Se uma das situações que dão início ao lançamento do dado acontecer...
  if(botao >= 900 || dist <= 10.0 || appRead == 1)  
	{  
		lcd.setCursor(0, 0);
		lcd.print("      DADO      ");
		lcd.setCursor(0, 1);
		lcd.print("    a Rolar...   ");

		animacaoLeds();
		randNum = random(1, 7);
 	
		desligaLeds();
		delay(700);

		switch(randNum)
		{
			case 1:
				digitalWrite(ledPin4 , HIGH);
				break;
    
			case 2:
				digitalWrite(ledPin2 , HIGH);
				digitalWrite(ledPin6 , HIGH);
				break;
    
			case 3:
				digitalWrite(ledPin2 , HIGH);
				digitalWrite(ledPin4 , HIGH);
				digitalWrite(ledPin6 , HIGH);
				break;
    
			case 4:
				digitalWrite(ledPin1 , HIGH);
				digitalWrite(ledPin3 , HIGH);
				digitalWrite(ledPin5 , HIGH);
				digitalWrite(ledPin7 , HIGH);
				break;
    
			case 5:
				digitalWrite(ledPin1 , HIGH);
				digitalWrite(ledPin3 , HIGH);
				digitalWrite(ledPin4 , HIGH);
				digitalWrite(ledPin5 , HIGH);
				digitalWrite(ledPin7 , HIGH);
				break;
    
			case 6:
				digitalWrite(ledPin1 , HIGH);
				digitalWrite(ledPin2 , HIGH);
				digitalWrite(ledPin3 , HIGH);
				digitalWrite(ledPin5 , HIGH);
				digitalWrite(ledPin6 , HIGH);
				digitalWrite(ledPin7 , HIGH);
				break;    
    }
  
		lcd.setCursor(0, 0);
		lcd.print("    NUMERO:   ");
		lcd.setCursor(0, 1);
		lcd.print("    << " + String(randNum) + " >>   ");
		animacaoBuzzer();
    
		delay(3000);
		desligaLeds();
    
		lcd.setCursor(0, 0);
		lcd.print("   Lancamento  ");
		lcd.setCursor(0, 1);
		lcd.print("   de um DADO  ");
		//delay(500);
	}
}

void desligaLeds(void)
{
	digitalWrite(ledPin1, LOW);
	digitalWrite(ledPin2, LOW);
	digitalWrite(ledPin3, LOW);
	digitalWrite(ledPin4, LOW);
	digitalWrite(ledPin5, LOW);
	digitalWrite(ledPin6, LOW);
	digitalWrite(ledPin7, LOW);
}

void animacaoLeds(void)
{
	int i;
  
	for (i = 0; i < compLista - 2; i++)
	{
		digitalWrite(listaAnim[i], HIGH);
		digitalWrite(listaAnim[i + 1], HIGH);
		digitalWrite(listaAnim[i +2], HIGH);
		digitalWrite(listaAnim[i - 1], LOW);
		delay(100);
	}

	for (i = compLista - 1; i > 1; i--)
	{
		digitalWrite(listaAnim[i], HIGH);
		digitalWrite(listaAnim[i - 1], HIGH);
		digitalWrite(listaAnim[i - 2], HIGH);
		digitalWrite(listaAnim[i + 1], LOW);
		delay(100);
	}
}

void animacaoBuzzer(void)
{
  	int i;
  
  for (i = 0; i < 4; i++)
	{
		tone(buzzerPin, tones[i]);
		delay(150);
		noTone(buzzerPin);	
	}
  
  for (i = 3; i >= 0; i--)
	{
		tone(buzzerPin, tones[i]);
		delay(150);
		noTone(buzzerPin);	
	}
}