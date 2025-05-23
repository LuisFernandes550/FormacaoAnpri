#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inicializa o LCD I2C no endereço 0x27 com 16 colunas e 2 linhas
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definição dos pinos dos botões
const int btn_inc = 2; // Incrementa pontos
const int btn_dec = 3; // Decrementa pontos
const int btn_rst = 4; // Reset pontos

// Definição dos pinos do sensor KY-033
const int sensorPin = 9;

// Definição do pino do buzzer
const int buzzer = 5;

int pontos = 0;

void atualizarLCD() 
{
  lcd.setCursor(0, 1);
  lcd.print("      "); // Limpa a linha
  lcd.setCursor(0, 1);
  lcd.print(pontos);
}

void setup() 
{
  pinMode(btn_inc, INPUT_PULLUP);
  pinMode(btn_dec, INPUT_PULLUP);
  pinMode(btn_rst, INPUT_PULLUP);
  pinMode(sensorPin, INPUT);
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);
  
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Pontos Equipa:");
  atualizarLCD();
}

void loop() 
{
  if (digitalRead(btn_inc) == LOW) 
  {
    pontos++;
    delay(200); // p/Debounce
    atualizarLCD();
  }
  
  if (digitalRead(btn_dec) == LOW && pontos > 0) 
  {
    pontos--;
    delay(200);
    atualizarLCD();
  }

  if (digitalRead(btn_rst) == LOW) 
  {
    pontos = 0;
    delay(200);
    atualizarLCD();
  }

  if (digitalRead(sensorPin) == LOW) 
  {
    digitalWrite(buzzer, HIGH);
    pontos++;
    atualizarLCD();
    delay(500);
    digitalWrite(buzzer, LOW);
  }

  Serial.print("Pontos: ");
  Serial.println(pontos);
   
  delay(50); // Para estabilizar leituras 
}