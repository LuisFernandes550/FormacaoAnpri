#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// Inicializações
Servo myservo;
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Endereço I2C do LCD

#define Senha 8 // 7 caracteres + '\0'
#define BUZZER_PIN 13
#define LED_VERDE 11
#define LED_VERMELHO 12

char Data[Senha];
char Master[Senha] = "1234567";  // Palavra-passe correta
byte data_count = 0;
bool porta = true;
char customKey;

// Teclado 4x4
const byte ROWS = 4;
const byte COLS = 4;

char hexakeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3, 2};

Keypad customKeypad = Keypad(makeKeymap(hexakeys), rowPins, colPins, ROWS, COLS);

void setup() {
  myservo.attach(10);     // Servo no pino digital 10
  myservo.write(180); //inicializa o servo na posição fechada (180 graus)
  delay(500); // estabilizar
  lcd.init();             // Inicializa o LCD
  lcd.backlight();        // Liga a luz de fundo

  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);

  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_VERMELHO, LOW);

  ServoClose();

  lcd.setCursor(0, 0);
  lcd.print("  Porta Arduino");
  lcd.setCursor(0, 1);
  lcd.print("--ProjetoAnpri--");
  delay(3000);
  lcd.clear();
}

void loop() {
  Open();
}

void clearData() {
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
}

void ServoOpen() {
  for (int pos = 180; pos >= 110; pos -= 5) {
    myservo.write(pos);
    delay(15);
  }
}

void ServoClose() {
  for (int pos = 0; pos <= 0; pos += 5) {
    myservo.write(pos);
    delay(15);
  }
}

void Open() {
  lcd.setCursor(0, 0);
  lcd.print("Insira a senha:");

  customKey = customKeypad.getKey();

  if (customKey) {
    Data[data_count] = customKey;
    lcd.setCursor(data_count, 1);
    lcd.print('*');  // Oculta a senha
    data_count++;
  }

  if (data_count == Senha - 1) {
    lcd.clear();

    if (strncmp(Data, Master, Senha - 1) == 0) {
      ServoOpen();
      digitalWrite(LED_VERDE, HIGH);
      digitalWrite(LED_VERMELHO, LOW);
      lcd.print(" Porta Aberta");
      delay(2000);
      lcd.clear();
      lcd.print(" Bem-vindos!");
      delay(2000);
      lcd.clear();

      // Recomeça o processo de senha após mostrar "Bem-vindos"
      ServoClose();
      digitalWrite(LED_VERDE, LOW);
      lcd.print("Insira a senha:");
      clearData();
      data_count = 0;
      delay(500); // pequeno atraso para evitar múltiplas leituras

    } else {
      lcd.print("Senha Errada!");
      tone(BUZZER_PIN, 1000);
      digitalWrite(LED_VERMELHO, HIGH);
      digitalWrite(LED_VERDE, LOW);
      delay(2000);
      noTone(BUZZER_PIN);
      digitalWrite(LED_VERMELHO, LOW);
      lcd.clear();
    }

    clearData();
    lcd.clear();
  }
}