y#include <Wire.h> // para trabalhar com I2C
#include <LiquidCrystal_I2C.h> // para trabalhar com o display
// #define CARKABUTTONPIN 12
// #define TECKABUTTONPIN 8
// #define ODDELBUTTONPIN 10

const int CARKABUTTONPIN = 12; //Traço
const int TECKABUTTONPIN = 7; //Ponto
const int ODDELBUTTONPIN = 10; //Espaço
const int DISPLAY_NUMOFCOLUMNS = 16;
int ledPin = 13;
int tonePin = 2;
int carkaButtonState = 0;
int carkaButtonLastState = 0;
int teckaButtonState = 0;
int teckaButtonLastState = 0;
int oddelButtonState = 0; //oddel = Departamento
int oddelButtonLastState = 0;
String tonesBuffer;
String text;
String expectedText;
String symbolsAlphabet[][2] =
{
	{ ".-","A" },
	{ "-...","B" },
	{ "-.-.","C" },
	{ "-..","D" },
	{ ".","E" },
	{ "..-.","F" },
	{ "--.","G" },
	{ "....","H" },
	{ "..","I" },
	{ ".---","J" },
	{ "-.-","K" },
	{ ".-..","L" },
	{ "--","M" },
	{ "-.","N" },
	{ "---","O" },
	{ ".--.","P" },
	{ "--.-","Q" },
	{ ".-.","R" },
	{ "...","S" },
	{ "-","T" },
	{ "..-","U" },
	{ "...-","V" },
	{ ".--","W" },
	{ "-..-","X" },
	{ "-.--","Y" },
	{ "--..","Z" },
	{ ".----","1" },
	{ "..---","2" },
	{ "...--","3" },
	{ "....-","4" },
	{ ".....","5" },
	{ "-....","6" },
	{ "--...","7" },
	{ "---..","8" },
	{ "----.","9" },
	{ "-----","0"}
};

LiquidCrystal_I2C lcd(0x27, DISPLAY_NUMOFCOLUMNS, 2);

char getToneFromButtonStates(){

	//retorna se o botão for liberado
	//ou seja, quando o estado atual é 0, o estado anterior é 1

  if (!carkaButtonState && carkaButtonLastState) {
		
        delay(50);
        digitalWrite(ledPin, HIGH); //turn on LED and tone //PF
        tone(tonePin, 1000); //PF
        delay(500);     //PF
        digitalWrite(ledPin, LOW); //torn off LED and tone //PF
        noTone(2); //PF
        return '-';
  }
  if (!teckaButtonState && teckaButtonLastState) {
		
    	delay(50);
        digitalWrite(ledPin, HIGH); //turn on LED and tone //PF
        tone(tonePin, 1000); //PF
        delay(150);     //PF
        digitalWrite(ledPin, LOW); //torn off LED and tone //PF
        noTone(2); //PF
        return '.';
  }
	if (!oddelButtonState && oddelButtonLastState)
		return ' ';

	return (char)0;
}

char getSymbolFromBuffer()
{
	if (tonesBuffer == "")
		return ' '; //criará um espaço se não houver caracteres antes

	for (int i = 0; i < sizeof symbolsAlphabet / sizeof symbolsAlphabet[0]; i++)
		//Examino todos os símbolos e comparo o buffer com o alfabeto
		if (tonesBuffer == symbolsAlphabet[i][0])
			return symbolsAlphabet[i][1][0];//se eu apenas retornar o símbolo fornecido

	//O buffer não corresponde a nenhum símbolo, então não enviarei nada
	return (char)0;
}

void extractActionFromTonesBuffer()
{
	if (tonesBuffer == "......")//6x ponto
		text.remove(text.length() - 1, 1);//exclui uma letra
	if (tonesBuffer == "------")//6x vírgula
		text = "";//exclui todo o texto
}

void setup() {

	lcd.init();
	lcd.backlight();
	lcd.print("Codigo Morse");
	lcd.setCursor(0, 1);
	lcd.print("6x.Apag1 6x-Tudo");

	pinMode(CARKABUTTONPIN, INPUT);
	pinMode(TECKABUTTONPIN, INPUT);
	pinMode(ODDELBUTTONPIN, INPUT);
}

void loop() {

	//načtení stavů tlačítek
	carkaButtonState = digitalRead(CARKABUTTONPIN);
	teckaButtonState = digitalRead(TECKABUTTONPIN);
	oddelButtonState = digitalRead(ODDELBUTTONPIN);

	char tone = getToneFromButtonStates();//detecta se foi premido um botão
    // e qual ele é
	if (tone != (char)0)
	{
		if (tone == ' ')//encerrando a sequência de tons, procurando um símbolo
		{
			char symbol = getSymbolFromBuffer();
          

			if (symbol != (char)0)//A sequência de tons encontrou um símbolo, está escrito no texto
			{
				text += symbol;
                  //digitalWrite(ledPin, HIGH); //turn on LED and tone //PF
                 //tone(tonePin, 1000); //PF
                  //delay(600);     //PF
                  //digitalWrite(ledPin, LOW); //torn off LED and tone //PF
                 // noTone(2); //PF
				if (text.length() > DISPLAY_NUMOFCOLUMNS)//Se o número de caracteres exceder o tamanho de exibição,
       //um novo caractere é escrito em primeiro lugar. Os outros serão excluídos.
				{
				  text = (String)symbol;
                  
				}
			}
			else//Uma sequência de tons não fornece nenhum símbolo, mas talvez alguma ação (como apagar um caractere)
			{
				extractActionFromTonesBuffer();
			}
			tonesBuffer = "";//vymaže se buffer (čárky a tečky)
		}
		else//traço ou ponto final
		{
			tonesBuffer += tone;
			if (tonesBuffer.length() > DISPLAY_NUMOFCOLUMNS)//se houver mais tons do que o tamanho do display, o buffer será limpo
			{
				tonesBuffer = (String)tone;
			}
		}

		//a escrita no display só é feita se um botão for pressionado

		lcd.clear();//o display é limpo
		lcd.print(text);//o texto está escrito
		lcd.setCursor(0, 1);
		lcd.print(tonesBuffer);//uma sequência de tons é escrita

	}

	//o estado anterior é atualizado
	carkaButtonLastState = carkaButtonState;
	teckaButtonLastState = teckaButtonState;
	oddelButtonLastState = oddelButtonState;

}
