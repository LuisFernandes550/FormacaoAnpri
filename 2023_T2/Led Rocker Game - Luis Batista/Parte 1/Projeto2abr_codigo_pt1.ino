const int ledPin[] = { 5, 6, 7, 8, 9, 10, 11, 12, 13 };

#define btnPin 4

int i, botao, pinAtual;

int wait = 500;
int vidas = 3;

void setup() {
  for (i = 0; i < 9; i++) {
    pinMode(ledPin[i], OUTPUT);
  }
  pinMode(btnPin, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.begin(9600);
  
    for (i = 0; i < 8; i++) {
      digitalWrite(ledPin[i], HIGH);
      pinAtual = i;
      //Serial.println(pinAtual);
      btnPress();
      delay(wait);
      digitalWrite(ledPin[i], LOW);
      btnPress();
      delay(wait);
    }
    

    for (i = 8; i > 0; i--) {
      digitalWrite(ledPin[i], HIGH);
      pinAtual = i;
      //Serial.println(pinAtual);
      btnPress();
      delay(wait);
      digitalWrite(ledPin[i], LOW);
      btnPress();
      delay(wait);
    }
    
  
}

void btnPress() {
  botao = digitalRead(btnPin);
  if (botao == 1) {
    //wait -= 50;
    //desligarPin();
    digitalWrite(ledPin[pinAtual], HIGH);
    Serial.print("Premiu led: ");
    Serial.println(pinAtual + 1);
    delay(100);
    digitalWrite(ledPin[pinAtual], HIGH);
    if (vidasTest() == 0) 
      {
        Serial.println("Fim do jogo");
        desligarPin();
        vidas = 3;
        delay(10000);
        Serial.println("Novo jogo, vidas 3");
      }
    wait /= 1.5;
    delay(4000);

  }
}

void desligarPin()
{
  for(i=0; i<9; i++) digitalWrite(ledPin[i], LOW);
}

int vidasTest()
{
  if ((pinAtual + 1) == 5)
  {
    Serial.println("Acertou, nível seguinte");
    vidas += 1;
    Serial.print("Vidas = ");
    Serial.println(vidas);
    
  }
  else {
    Serial.println("Errou, nível menor");
    vidas -= 1;
    Serial.print("Vidas = ");
    Serial.println(vidas);
    
  }
  return vidas;
}







