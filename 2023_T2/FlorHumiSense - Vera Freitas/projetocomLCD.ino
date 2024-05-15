#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>


#include <Wire.h>

LiquidCrystal_I2C lcd(0x27,16,2);
SoftwareSerial bluetooth(0, 1); // RX, TX
#define humidade A0
#define pino_led_vermelho 5
#define pino_led_amarelo 6
#define pino_led_verde 7
 
int valor_analogico;
 
void setup()
{
  lcd.init();                      // Inicializa o LCD
  lcd.backlight();                // Liga a luz de fundo do LCD
  Serial.begin(9600);
  pinMode(humidade, INPUT);
  pinMode(pino_led_vermelho, OUTPUT);
  pinMode(pino_led_amarelo, OUTPUT);
  pinMode(pino_led_verde, OUTPUT);

  
 
}
 
void loop()
{
  // Le o valor do pino A0 do sensor
  valor_analogico = analogRead(humidade);
 
  // Mostra o valor da porta analógica no display LCD
  lcd.clear();                           // Limpa o LCD
  lcd.setCursor(0, 0);            // Define a posição do cursor na primeira linha
  lcd.print("Humidade: ");
  lcd.print(valor_analogico);
  
  // Solo umido, acende o led verde
  if (valor_analogico > 0 && valor_analogico < 500)
  {
    Serial.println(" Status: Solo humido");
    apagaleds();
    digitalWrite(pino_led_verde, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Solo humido");
  }
 
  // Solo com umidade moderada, acende led amarelo
  if (valor_analogico > 500 && valor_analogico < 800)
  {
    Serial.println(" Status: Humidade moderada");
    apagaleds();
    digitalWrite(pino_led_amarelo, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Humidade moderada");
  }
 
  // Solo seco, acende led vermelho
  if (valor_analogico > 800 && valor_analogico < 1024)
  {
    Serial.println(" Status: Solo seco!!Regar");
    apagaleds();
    digitalWrite(pino_led_vermelho, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Solo seco!!Regar");
  }
    if (bluetooth.available()) {
    char dadoRecebido = bluetooth.read(); // Lê o dado recebido do Bluetooth
    Serial.write(dadoRecebido); // Envia o dado recebido para o computador
  }

  // Verifica se há dados disponíveis para leitura do computador
  if (Serial.available()) {
    char dadoEnviado = Serial.read(); // Lê o dado enviado pelo computador
    bluetooth.write(dadoEnviado); // Envia o dado para o módulo Bluetooth
  }
  delay(100);
}
 
void apagaleds()
{
  digitalWrite(pino_led_vermelho, LOW);
  digitalWrite(pino_led_amarelo, LOW);
  digitalWrite(pino_led_verde, LOW);
}
