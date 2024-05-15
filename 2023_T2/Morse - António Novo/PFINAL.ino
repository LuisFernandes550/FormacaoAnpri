// PROJETO MORSE
#define BUZZERPIN  13 // Pin out do buzzer
#define MORSEPIN   12 // Pin out do led branco
#define MSGPIN     11 // Pin out do led vermelho
#define POTPIN     A0 // Pin in do potenciometro
const int BTNPIN[] = {2,4,6,8}; // Pin in dos botões de pressão
const int LEDPIN[] = {3,5,7,9}; // Pin out dos leds dos botões de pressão

// temporizações
#define DITMIN  50    // duração inicial do ponto (DIT) em milisegundos
#define POTMIN  -30   // valor mínimo do potenciómetro (para mapeamento)
#define POTMAX  120   // valor máximo do potenciómetro (para mapeamento)
int duracaoDITS; // DITMIN + valor do potenciometro (POTMIN..POTMAX)
int duracaoDAHS; // 3 * duracaoDITS
// Intervalo entre pontos e traços 1 DIT
// Intervalo curto (entre letras) 1 DAH
// Intervalo médio (entre palavras) 7 DITS
// Intervalo longo (entre frases) 4 DAHS

// Tons do buzzer 
const int TONS[] = {262,349,494}; // Do,Fa,Si

// Código Morse
// ascii números 48..57 '0'..'9'
const String numMORSE[] = { "-----",
                            ".----",
                          	"..---",
                          	"...--",
                          	"....-",
                          	".....",
                          	"-....",
                          	"--...",
                          	"---..",
                          	"----." };
// ascii letras 65..90 'A'..'Z'
const String chrMORSE[] = { ".-",	// A
                            "-...",
                          	"-.-.",
                          	"-..",
                          	".",
                          	"..-.",
                          	"--.",
                          	"....",
                          	"..",
                            ".---",	// J
                          	"-.-",
                          	".-..",
                          	"--",
                          	"-.",
                          	"---",
                          	".--.",
                          	"--.-",                           
                            ".-.",	// R
                          	"...",
                          	"-",
                          	"..-",
                          	"...-",
                          	".--",
                          	"-..-",
                          	"-.--",                           
                          	"--.." }; // Z
// ascii espaço  32 ' '
// ascii outros  ',' '.' '?' ';' ':' ''' '-' '/' '(' ')'


int potValue = 0, potAnt = -10;
int btnOn = 0, btnAnt = -1;
bool msgOn = false;
String msg;

void setup()
{
  for(int i = 0; i < 4 ; i++)
  {
    pinMode(BTNPIN[i],INPUT);
    pinMode(LEDPIN[i],OUTPUT);
  }
  pinMode(BUZZERPIN,OUTPUT);
  pinMode(MORSEPIN,OUTPUT);
  pinMode(MSGPIN,OUTPUT);
  pinMode(POTPIN,INPUT);
  Serial.begin(9600);
}

void loop()
{
  	if( !msgOn )  // Se não estiver a processar nenhuma mensagem
    {	// verifica estado dos botões de pressão
      for (int i = 0; i < 4; i++)
          if(digitalRead(BTNPIN[i]))
          {   btnOn = i;
              break;
          }
      if (btnOn != btnAnt)
      {
          for(int i = 0; i < 4; i++)
            if(i == btnOn)
            	digitalWrite(LEDPIN[btnOn],HIGH);
            else
            	digitalWrite(LEDPIN[i],LOW);
          btnAnt = btnOn;
          if(btnOn != 3)
          {
          	Serial.print("Tom : ");
          	Serial.println(TONS[btnOn]);
          }
      }
      // verifica valor do potenciometro
      potValue = map(analogRead(POTPIN),0,1023,POTMIN,POTMAX);
      if (potValue != potAnt)
      {
          duracaoDITS = DITMIN+potValue;
          duracaoDAHS = 3 * duracaoDITS;
          potAnt = potValue;
          Serial.print("Duração : ");
          Serial.print(duracaoDITS);
          Serial.println(" ms");
      }
      // verifica existencia de mensagem
      if (Serial.available())
      {
          msg = Serial.readString();
          if (msg != "")
            msgOn = true;
      }
  }
  else  // existe uma mensagem a processar
  {
    digitalWrite(MSGPIN,HIGH);
    processa_mensagem(msg);
    msg = "";
    msgOn = false;
    digitalWrite(MSGPIN,LOW);
  }
}

void processa_mensagem(String msg)
{
  char c;
  int i, nc;
  String codMorse;

  msg.toUpperCase();
  nc = msg.length();
  Serial.println(msg);  
  for(i = 0; i < nc; i++)
  {
    c = msg[i]; // carater a processar
    Serial.println(c);

    if(c == ' ')  // espaço entre palavras (médio) = 7 DITS
      	delay(7*duracaoDITS);
    else if (c == '.') // final de frase (longo) =  4 DAHS
      	delay(4*duracaoDAHS);
    else
    {
      if(i!=0)
      	delay(duracaoDAHS);  // espaço entre letras (curto)
      codMorse = "";
      if (c >= '0' && c <= '9')
      	codMorse = numMORSE[c-'0'];
      else if (c >= 'A' && c <= 'Z')
        codMorse = chrMORSE[c-'A'];
      Serial.println(codMorse);
      morse(codMorse);
    }
  }
}

void morse(String codmorse)
{
  char c;
  int i, nc = codmorse.length();
  int milis;
  
  for(i = 0; i < nc; i++)
  {
    if (i!=0) 
      delay(duracaoDITS);   // delay entre traços e pontos
    c = codmorse[i];
    if(c == '-') // traço
		milis = duracaoDAHS;
    else		// ponto
      	milis = duracaoDITS;
    
    if(btnOn==3) // morse luminoso
      	digitalWrite(MORSEPIN,HIGH);
    else		// morse sonoro
      	tone(BUZZERPIN,TONS[btnOn]);
    delay(milis); // duração
    if(btnOn==3)
	    digitalWrite(MORSEPIN,LOW);
    else
    	noTone(BUZZERPIN);
  }
}

