#include <LiquidCrystal_I2C.h> 
#include <Wire.h>
#include <Servo.h>
#include <Key.h>
#include <Keypad.h>
#define Password_Length 5  //constante definida tamanho password



int ledState = 0;
boolean flag = true;

Servo myServo;  //Servo motor

LiquidCrystal_I2C lcd(0x27, 16, 4); //inicialização do LCD

int PB = 12, LED = 13;  //pinos do pushbutton e do LED
int LED_State = 0;  //LED da porta arranca desligado
int button_old = 1;
int button_new;

int pos = 0;
int posIni = 90;

char Data[Password_Length];             //password escrita
char Master[Password_Length] = "1111";  //password estátca do sistema
byte data_count = 0, master_count = 0;  //dados recebidos

bool PassOK;         //password correcta
bool porta = false;  //door
char customKey;

/*configuração do teclado (keypad) */

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte rowPins[ROWS] = { 2, 3, 4, 5 };
byte colPins[COLS] = { 6, 7, 8, 9 };

Keypad customKeypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);  //configuração do teclado 4x4


/*fim configuração KeyPad
*/

void setup() {
  Serial.begin(9600);  // debugging a 9600!

  myServo.attach(10);  //agarrar o pino 10 ao motor

  pinMode(LED, OUTPUT);  // indicação de que o LED serve para OUTPUT
  pinMode(PB, INPUT);    //indicação de que o caminho PB -> press button é para input. Controla ligar o LED e ativar o serviço da porta
  myServo.write(90);     //posição inicial da porta.
  lcd.init();            //inicializar o lcd
  delay(150);            //pausa/atraso para garantir que o ARDUINO recebeu e processou o estado inicial
  myServo.detach();      //Sleep servo! desligar o ontrolo para que não haja sinal por correntes acidentais
}

void loop() {
  button_new = digitalRead(PB);  //leitura do estado do PRESSBUTTON

  delay(50);

  if (button_old == 1 && button_new == 0) {
    if (LED_State == 0) {       // o esrado do LED era desligado. deerá passar a LIGADO
      digitalWrite(LED, HIGH);  //LED passa a ligado
      lcd.clear();              // limpeza do LCD
      delay(50);
      LED_State = 1;    // variável de controlo do estado atual do LED a 1
      lcd.backlight();  //ligar a luz do LCD
                        //lcd.setCursor(0,0);
      //lcd.begin(16, 2);
      Serial.write("preparar para abrir \n");

      delay(100);
      lcd.print("Porta protegida  kkkk");
      loading("Loading");
      //lcd.clear();
      delay(1000);

      // lcd.print("Porta Protegida");
      lcd.clear();
      //Open(); NÃO PODE SER AQUI
    } else {
      digitalWrite(LED, LOW);
      LED_State = 0;
      lcd.noBacklight();
      button_old = 1;
      button_new = 1;
      delay(150);
    }
    button_old = button_new;
  } else {
    button_old = button_new;
    //Serial.print("button_old ");
    //Serial.println(button_old);
    button_old = 1;
    button_new = 1;
  }

  if (porta == true) {
    customKey = customKeypad.getKey();
    if (customKey == '#') {
      lcd.clear();
      ServoClose();
      lcd.print("CLOSING DOOR 222222");
      delay(3000);
      porta = false;
      delay(150);
      lcd.noBacklight();
      delay(150);
    }
  } else {
    Serial.write("preparar para abrir \n");
    Open();
  }

}  // end of main loop.



//funç~~oes auxiliares de controlo equipamento

void loading(char msg[]) {
  lcd.setCursor(0, 1);
  lcd.print(msg);

  for (int i = 0; i < 9; i++) {
    delay(1000);
    lcd.print(".");
  }
}

void clearData() {
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}

void ServoClose() {
  //Serial.write(myServo.read());
  for (pos = 0; pos < 90; pos++) {
    myServo.write(pos);
    delay(10);
    Serial.print("fechar");
    Serial.print(pos);
  }
  LED_State = 0;
  digitalWrite(PB, -1);  //leitura do estado do PRESSBUTTON

  delay(50);
}

void ServoOpen() {
  for (pos = 90; pos >= 1; pos--) {
    myServo.write(pos);
    delay(10);
  }
}

void Open() {
  if (LED_State == 1) {
    //lcd.clear();
    myServo.attach(10);
    delay(150);
    lcd.setCursor(0, 0);
    lcd.print("Introduza PIN :");

    customKey = customKeypad.getKey();  //leitura de um valor do teclado
    if (customKey) {
      Data[data_count] = customKey;
      lcd.setCursor(data_count, 1);
      lcd.print(Data[data_count]);
      data_count++;
    }

    if (data_count == Password_Length - 1) {
      if (!strcmp(Data, Master)) {  // comparar os valores recebidos com o valor exato secreto
        lcd.clear();
        delay(50);
        lcd.print(" Abrir Porta ");
        loading("Aguarde");
        ServoOpen();
        porta = true;
        delay(5000);

        lcd.clear();
        lcd.print("Fechar Porta ");

        ServoClose();
        delay(2000);
        porta = false;
        myServo.detach();
        delay(100);
        digitalWrite(PB, LOW);
        digitalWrite(LED, LOW);

        delay(150);

        LED_State = 0;
        button_new = 0;
        delay(150);
        ledState = 0;

        lcd.noBacklight();
        delay(150);

      } else {
        lcd.clear();
        lcd.print(" Erro!!!");
        porta = false;
        delay(3000);
      }
      //delay(5000);
      lcd.clear();
      clearData();
    }
  } else {
    Serial.write("nada a fazer aqui\n");
  }
}