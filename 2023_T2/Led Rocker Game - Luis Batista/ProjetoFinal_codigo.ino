#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int ledPin[] = { 5, 6, 7, 8, 9, 10, 11, 12, 13 };

#define buzzerPin 3

#define btnPin 4

int i, botao, pinAtual;

int wait = 500;
int vidas = 3;

void setup() {
  for (i = 0; i < 9; i++) {
    pinMode(ledPin[i], OUTPUT);
  }
  pinMode(btnPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  lcd.init();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Serial.begin(9600);

  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("LED Rocker Game");
  lcd.setCursor(0, 1);
  
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
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Premiu led: ");
    Serial.println(pinAtual + 1);
    lcd.setCursor(0, 1);
    lcd.print(pinAtual + 1);
    delay(100);
    digitalWrite(ledPin[pinAtual], LOW); //Os vídeos a executar tinham este led ligado, para ver que estava a detetar corretamente
    wait /= 1.5;
    if (vidasTest() == 0) 
      {
        Serial.println("Fim do jogo");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Fim do jogo");
        desligarPin();
        tone(buzzerPin, 349);
        delay(50);
        tone(buzzerPin, 330);
        delay(50);
        tone(buzzerPin, 294);
        delay(50);
        tone(buzzerPin, 262);
        delay(200);
        noTone(buzzerPin);
        vidas = 3;
        delay(10000);
        wait = 500;
        Serial.println("Novo jogo, vidas 3");
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Novo jogo");
        lcd.setCursor(0, 1);
        lcd.print("3 vidas");
      }
    //wait /= 1.5;
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
    tone(buzzerPin, 349);
    delay(50);
    noTone(buzzerPin);
    Serial.println("Acertou, nível+");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Acertou, nível+");
    
    vidas += 1;
    Serial.print("Vidas = ");
    //lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Vidas = ");
    Serial.println(vidas);
    lcd.print(vidas);    
  }
  else {
    tone(buzzerPin, 262);
    delay(200);
    noTone(buzzerPin);
    Serial.println("Errou, nível-");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Errou, nível-");
    vidas -= 1;
    Serial.print("Vidas = ");
    //lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Vidas = ");
    Serial.println(vidas);
    lcd.print(vidas);
  }
  return vidas;
}




