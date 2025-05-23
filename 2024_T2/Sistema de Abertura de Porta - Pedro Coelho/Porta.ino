#include <Key.h>
#include <Wire.h>
#include <Keypad.h>
#include <Servo.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>

// Pino RX do módulo Bluetooth
const int RX_PIN = 11;
// Pino TX do módulo Bluetooth
const int TX_PIN = 10;
// Pino do LED
int pos = 0;

//const int LED_PIN = 13;
SoftwareSerial bluetooth(RX_PIN, TX_PIN);
//int Button = 10;           //Botão

const byte numRows = 4;         //número de linhas no teclado
const byte numCols = 4;         //número de colunas no teclado

char keymap[numRows][numCols] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

char keypressed;                 //O local onde as chaves são armazenadas
char code[] = {'1', '2', '3', '4'}; //O código padrão, pode ser alterado ou torná-lo um código de 'n' dígitos

char check1[sizeof(code)];  //Onde a nova chave é armazenada
char check2[sizeof(code)];  //Onde a nova chave é armazenada novamente para ser comparada com a anterior

short a = 0, i = 0, s = 0, j = 0; //Variáveis ​​usadas

byte rowPins[numRows] = {2, 3, 4, 5}; //Linhas 0 a 3 
byte colPins[numCols] = {6, 7, 8, 9}; //Columas 0 to 3

LiquidCrystal_I2C lcd(0x27, 16, 2);
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);
Servo myservo;

void setup()
{
  lcd.init();                      // inicia o lcd
  lcd.backlight();
  lcd.begin (16, 2);
  lcd.setCursor(0, 0);
  lcd.print("DIGITE O CODIGO");
  lcd.setCursor(1 , 1);
  lcd.print("Para abrir ...!!");      //O que está escrito no LCD você pode alterado
  //pinMode(Button, INPUT);
  myservo.attach(10);
  myservo.write(130);
  //CloseDoor();
  
  bluetooth.begin(9600);
  Serial.begin(9600);

  //  for(i=0 ; i<sizeof(code);i++){        //Quando carregar o código pela primeira vez
  //            EEPROM.get(i, code[i]);             //Carrega o código e altere-o para o armazenar na EEPROM
  //           }                                  //(isso é feito apenas uma vez)


}


void loop()
{

  keypressed = myKeypad.getKey();             //Espera que uma tecla seja pressionada
  if (keypressed == '*') {                    // * para abrir a
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.println("DIGITE O CODIGO");            
    ReadCode();                          //Obtendo função de código
    if (a == sizeof(code))               //A função ReadCode atribui um valor a 'a' (está correto quando tem o tamanho do array de código)
      OpenDoor();                        //Função de bloqueio aberto se o código estiver correto
    else {
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("CODIGO");    //Mensagem para imprimir quando o código estiver errado
      lcd.setCursor(6, 0);
      lcd.print("ERRADO");
      lcd.setCursor(15, 1);
      lcd.print(" ");
      lcd.setCursor(4, 1);
      lcd.print("Invalido!!!");
    }
    delay(5000);
    CloseDoor();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DIGITE O CODIGO");
    lcd.setCursor(1 , 1);
    lcd.print("* para digitar!!!!");     //Retornar ao modo de espera é a mensagem que aparece quando se espera
  }

  if (keypressed == '#') {                //To change the code it calls the changecode function
    ChangeCode();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("DIGITE O CODIGO");
    lcd.setCursor(1 , 1);
    lcd.print("* para digitar!!");         //Quando concluído, ele retorna ao modo de espera
  }



  if (bluetooth.available()) {
    char comando = bluetooth.read();
    Serial.println(comando);
    switch (comando) {
      case 'A': // LED ON
        //Serial.println(comando);
        OpenDoor(); 
        //myservo.write(90);
        break;        
      case 'F': // LED OFF
        CloseDoor();
        //myservo.write(0);
        break;
    }
  }

  //if (digitalRead(Button) == HIGH) {  //Abertura pelo botão
  //  myservo.write(90);
  //}

}

void ReadCode() {           //Obter a sequência do código
  i = 0;                    //limpar as variáveis
  a = 0;
  j = 0;

  while (keypressed != 'A') {                                   //Pressiona A para confirmar o código, caso contrário continua a aceitar números
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A' ) {     //Se o char digitado for diferente de A
      lcd.setCursor(j, 1);                                 //Isto escreve "*" no LCD sempre que uma tecla é pressionada, a posição é controlada por j
      lcd.print("*");
      j++;
      if (keypressed == code[i] && i < sizeof(code)) {       //se o caractere digitado estiver correto, a e i são incrementados para verificar o próximo caractere
        a++;
        i++;
      }
      else
        a--;                                               //se o caractere digitado estiver errado, diminui e não pode ser igual ao tamanho do code[]
    }
  }
  keypressed = NO_KEY;

}

void ChangeCode() {                     //Change code sequence
  lcd.clear();
  lcd.print("Alterar codigo");
  delay(1000);
  lcd.clear();
  lcd.print("Codigo antigo");
  ReadCode();                      //verify the old code first so you can change it

  if (a == sizeof(code)) {  //again verifying the a value
    lcd.clear();
    lcd.print("Alterar codigo");
    GetNewCode1();            //Obtenha o novo código
    GetNewCode2();            //Obtenha o novo código para confirmação
    s = 0;
    for (i = 0 ; i < sizeof(code) ; i++) { //Comparar códigos na matriz 1 e na matriz 2 
      if (check1[i] == check2[i])
        s++;            
    }
    if (s == sizeof(code)) {        //O tamanho correto do array
      for (i = 0 ; i < sizeof(code) ; i++) {
        code[i] = check2[i];       //a matriz do código recebe o novo código
        EEPROM.put(i, code[i]);        //E armazena na EEPROM

      }
      lcd.clear();
      lcd.print("Codigo alterado");
      delay(2000);
    }
    else {                        //Caso os novos códigos não sejam correspondentes
      lcd.clear();
      lcd.print("Codigo nao ");
      lcd.setCursor(0, 1);
      lcd.print("encontrado !!");
      delay(2000);
    }

  }

  else {                    //Caso o código antigo esteja errado, o código atual não pode alterado
    lcd.clear();
    lcd.print("Errado");
    delay(2000);
  }
}

void GetNewCode1() {
  i = 0;
  j = 0;
  lcd.clear();
  lcd.print("Novo codigo");   //Pede para inserir o novo código 
  lcd.setCursor(0, 1);
  lcd.print("pressione A");           //e pressionar A
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("pressione A");     

  while (keypressed != 'A') {          //A para confirmar e sair do loop
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A' ) {
      lcd.setCursor(j, 0);
      lcd.print("*");               //No novo código mostra * 
      check1[i] = keypressed;   //Armazena caracteres na matriz
      i++;
      j++;
    }
  }
  keypressed = NO_KEY;
}

void GetNewCode2() {     //Isto é igual à função GetNewCode1, mas desta vez o código é armazenado em outra matriz
  i = 0;
  j = 0;

  lcd.clear();
  lcd.print("Confirmar codigo");
  lcd.setCursor(0, 1);
  lcd.print("pressione A");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("pressione A");

  while (keypressed != 'A') {
    keypressed = myKeypad.getKey();
    if (keypressed != NO_KEY && keypressed != 'A' ) {
      lcd.setCursor(j, 0);
      lcd.print("*");
      check2[i] = keypressed;
      i++;
      j++;
    }
  }
  keypressed = NO_KEY;
}

void CloseDoor() {            //Função de abre a porta
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Acesso concedido");
  lcd.setCursor(4, 1);
  lcd.print("Fechar porta!!");
  //myservo.write(130);
  for (pos = 0; pos <= 130; pos += 1) { 
    // in steps of 1 degree
    myservo.write(pos);             
    delay(1);                       
    }
  }
void OpenDoor() {            //Função de fecha a porta
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Acesso concedido");
  lcd.setCursor(4, 1);
  lcd.print("Abrir porta!!");
  //myservo.write(130);
  for (pos = 130; pos >= 0; pos -= 1) { 
    myservo.write(pos);              
    delay(1);                       
    }
  }
  //myservo.write(0);



