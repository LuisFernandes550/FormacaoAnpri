#include <SoftwareSerial.h>  // Libraria do módulo Bluetooth (HC-05)
#include <dht.h>
//#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
#define DHT11_PIN 7
dht DHT;

SoftwareSerial BTserial(11, 12); // RX, TX

#define botao1 2
#define botao2 3
#define botao3 4
#define botao4 5
#define resist_aquec 8
#define cooler 9
#define cooler_PWM 10
#define eletrovalvula 13

int estadobotao1 = 0;
int estadobotao2 = 0;
int estadobotao3 = 0;
int estadobotao4 = 0;

// Definicao de variaveis auxiliares
int menu = 0;
int temperatura;
int humidade;

int temp_max = 30;
int temp_min = 16;
int hum_max = 95;
int hum_min = 80;

int conta = 0;

// Setup *************************************************
void setup() {
  int chk = DHT.read11(DHT11_PIN);
  lcd.backlight();
  lcd.init();
  lcd.setCursor(0, 0);  // coluna 0  - linha 0
  lcd.print("Estufa");
  delay(1000);
  Serial.begin(9600);
  BTserial.begin(9600);  // Inicialização do Módulo Bluetooth

  // Definiçao dos pinos como Entrada ou Saida ************
  pinMode(cooler, OUTPUT);
  pinMode(cooler_PWM, OUTPUT);
  pinMode(resist_aquec, OUTPUT);
  pinMode(eletrovalvula, OUTPUT);
  pinMode(botao1, INPUT);
  pinMode(botao2, INPUT);
  pinMode(botao3, INPUT);
  pinMode(botao4, INPUT);
  menu = 0;  // Inicia no menu 0
  // Desliga saidas (desativo a nivel alto)
  digitalWrite(cooler, LOW);
  digitalWrite(resist_aquec, LOW);
}

void loop() {

  bluetooth_emissor();
  bluetooth_recetor();

  estadobotao1 = digitalRead(botao1);
  estadobotao2 = digitalRead(botao2);
  estadobotao3 = digitalRead(botao3);
  estadobotao4 = digitalRead(botao4);

  temperatura = DHT.temperature;
  humidade = DHT.humidity;

  if (conta == 20) {
    conta = 0;
  }

  // Restringe Valores da temp_max e temp_min ************
  if (temp_min >= temp_max) temp_max = temp_min + 5;
  if (hum_min >= hum_max) hum_max = hum_min + 5;

  // Controlo dos relés de saída (Resistencia de Aquecimento e Cooler Refrigeração
  if (temperatura < temp_min) {
    digitalWrite(resist_aquec, HIGH);
  }

  if (temperatura > temp_max) {
    digitalWrite(resist_aquec, LOW);
  }

  if (temperatura < temp_max) {
    digitalWrite(cooler_PWM, LOW);
  }

  
  if (temperatura > (temp_max + 2)) {  //Se temp acima 2ºC da Temp_Max
    digitalWrite(cooler, HIGH);
    analogWrite(cooler_PWM, 190);          // cooler roda a 50% da sua velocidade máx
  }

  if (temperatura > (temp_max + 5)) {  //Se temp acima 5ºC da Temp_Max
    digitalWrite(cooler, HIGH);
    analogWrite(cooler_PWM, 255);          // cooler roda a 100% da sua velocidade máx
  }
  ;

  // Controlo da Humidade

  if (humidade < hum_min) {
    digitalWrite(eletrovalvula, HIGH);
  }

  if (humidade > hum_max) {
    digitalWrite(eletrovalvula, LOW);
  }
  //***************************************************************

  // Acerto dos Parametros da temp_max, temp_min, hum_max e hum_min
  // com a utilizaçao de menus

  if (estadobotao1 == HIGH) {
    menu = menu + 1;
    delay(100);
  }

  if (estadobotao2 == HIGH) {
    menu = menu - 1;
    delay(100);
  }

  if (estadobotao3 == HIGH) {
    if (menu == 1) temp_min = temp_min + 1;
    if (menu == 2) temp_max = temp_max + 1;
    if (menu == 3) hum_min = hum_min + 1;
    if (menu == 4) hum_max = hum_max + 1;
    delay(80);
  }

  if (estadobotao4 == HIGH) {
    if (menu == 1) temp_min = temp_min - 1;
    if (menu == 2) temp_max = temp_max - 1;
    if (menu == 3) hum_min = hum_min - 1;
    if (menu == 4) hum_max = hum_max - 1;
    delay(80);
  }

  // Impressão da Temperatura
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperatura);
  lcd.write((char)223);
  lcd.print("C");
  delay(100);

  // Impressao da Humidade relativa
  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidade);
  lcd.print("%");
  delay(100);

  lcd.setCursor(7, 0);
  lcd.print("m");
  lcd.print(temp_min);

  lcd.setCursor(13, 0);
  lcd.print("M");
  lcd.print(temp_max);

  lcd.setCursor(7, 1);
  lcd.print("m");
  lcd.print(hum_min);
  delay(80);
  lcd.setCursor(13, 1);
  lcd.print("M");
  lcd.print(hum_max);
  delay(80);


  if (menu == 1) {
    lcd.setCursor(7, 0);
    lcd.print("m");
    delay(80);
    lcd.print("  ");
    delay(80);
  }

  if (menu == 2) {

    // Acerto dos Parametros temp_max e temp_min ***************
    lcd.setCursor(13, 0);
    lcd.print("M");
    delay(80);
    lcd.print("  ");
    delay(80);
  }


  // Impressao dos Parametros hum_max e hum_min *****************

  if (menu == 3) {
    lcd.setCursor(7, 1);
    lcd.print("m");
    delay(80);
    lcd.print("  ");
    delay(80);
  }

  if (menu == 4) {
    lcd.setCursor(13, 1);
    lcd.print("M");
    delay(80);
    lcd.print("  ");
    delay(80);
  }


  // Limita numero de menus ******************

  if (menu > 4) {
    menu = 4;  // mantem valor 4
  }
  if (menu < 0) {
    menu = 0;  // mantem valor 0
  }
}
