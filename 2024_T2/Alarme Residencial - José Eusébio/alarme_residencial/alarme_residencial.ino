#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <Keypad.h> //biblioteca do teclado matricial

#define LEDPIN 11
#define BUZZERPIN 13
#define PIRPIN 12

const byte LINHAS = 4; //Quantidade de linhas do teclado
const byte COLUNAS = 4; //Quantidade de colunas do teclado

//Matriz de caracteres referente aos botões do teclado
char TECLAS[LINHAS][COLUNAS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte PINOS_LINHAS[LINHAS] = {9, 8, 7, 6}; //Pinos das linhas
byte PINOS_COLUNAS[COLUNAS] = {5, 4, 3, 2}; //Pinos das Colunas


//Cria um objeto do tipo Keypad
Keypad teclado = Keypad(makeKeymap(TECLAS), PINOS_LINHAS, PINOS_COLUNAS, LINHAS, COLUNAS); //iniciação do teclado

LiquidCrystal_I2C lcd(0x27,16,2);  // Endereço 0x27 E CONFIGURAÇÃO DO DISPLAYset 

int estadoPIR = 0;

String senhaAtual = "1234";
String senhaDigitada;
int contaDigitos; 
int estado = 0;


void setup(){
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  lcd.clear();// LIMPA DISPLAY
 
  Serial.begin(9600); //Inicializar o serial monitor
 
  //Definir pinos como de entrada ou de saída
  pinMode(LEDPIN,OUTPUT);
  pinMode(BUZZERPIN,OUTPUT);
  pinMode(PIRPIN,INPUT);

  senhaDigitada = "";
  contaDigitos = 0;

  lcd.setCursor(0,0);
  lcd.print("   Iniciando  "); //MENSAGEM INICIAL
  lcd.setCursor(0,1);
  lcd.print("Sistema Alarme"); //MENSAGEM INICIAL
  delay(2000);
}


void loop(){
  char teclaLida;

    switch (estado)  {

      case 0:
        digitalWrite (BUZZERPIN, LOW);
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Ativar sistema");
        lcd.setCursor(0,1);
        lcd.print("Senha: ");
        estado = 1;
        break;

    case 1:
      //ações do estado E1
      digitalWrite (BUZZERPIN, LOW);
      teclaLida = teclado.getKey();
      Serial.println((char)teclaLida);
        
      if(teclaLida) {
        if(teclaLida == '*'){
          estado = 4;
          break;
        }
        else if (contaDigitos < 4) {
          senhaDigitada = senhaDigitada + teclaLida;
          lcd.print("*");
          contaDigitos = contaDigitos + 1;
        }                                          
        else {
          if (teclaLida == '#'){
            if (senhaDigitada == senhaAtual) {
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Sistema ativado!");
              lcd.setCursor(0,1);
              lcd.print("Senha: ");
              delay(1000);;
              estado = 2;
              contaDigitos = 0;
              senhaDigitada = "";
              break;
            }
            else{
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Senha incorreta! ");
              lcd.setCursor(0,1);
              lcd.print("Senha: ");
              delay(1000);
              estado = 0;
              contaDigitos = 0;
              senhaDigitada = "";
              break;
            }
          }
        }
      }
      break;
                            
    case 2:
      // ações estado E2
      digitalWrite (BUZZERPIN, LOW);
      if (digitalRead (PIRPIN) == HIGH) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("   !!ALARME!!    ");
        lcd.setCursor(0,1);
        lcd.print("Senha: ");                 
        estado = 3;
        contaDigitos = 0;
        senhaDigitada = "";
        //ligarAlarme();
        break;
      }
      teclaLida = teclado.getKey();
      
      if(teclaLida) {
        if (contaDigitos < 4) {
          senhaDigitada = senhaDigitada + teclaLida;
          lcd.print("*");
          contaDigitos = contaDigitos + 1;
        }                                                 
        else {
          if (teclaLida == '#'){
            if (senhaDigitada == senhaAtual) {
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Alarme desligado");
              lcd.setCursor(0,1);
              lcd.print("Senha: ");
              delay(1000);
              estado = 0;
              contaDigitos = 0;
              senhaDigitada = "";
              break;              
            }
            else{
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("Senha incorreta! ");
              lcd.setCursor(0,1);
              lcd.print("Senha: ");
              delay(1000);
              estado = 2;
              contaDigitos = 0;
              senhaDigitada = "";
              break;
            }
          }
        }
      }
      break;

      case 3:
        //ações do estado E3
        //digitalWrite (BUZZERPIN, HIGH);
        tone(BUZZERPIN,700);
        digitalWrite(LEDPIN, HIGH);
        teclaLida = teclado.getKey();
      
        if(teclaLida) {
          if (contaDigitos <4) {
            senhaDigitada = senhaDigitada + teclaLida;
            lcd.print("*");
            contaDigitos = contaDigitos + 1;
          }                                                 
          else {
            if (teclaLida == '#'){
              if (senhaDigitada == senhaAtual) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Alarme desligado");
                lcd.setCursor(0,1);
                lcd.print("Senha:");
                delay(1000);
                estado = 0;
                contaDigitos = 0;
                senhaDigitada = "";
                noTone(BUZZERPIN);
                digitalWrite(LEDPIN, LOW);
                break;              
              }
              else{
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print("Senha incorreta! ");
                delay(1000);
                lcd.setCursor(0,0);
                lcd.print("   !!ALARME!!    ");
                lcd.setCursor(0,1);
                lcd.print("Senha: ");   
                delay(1000);
                estado = 3;
                contaDigitos = 0;
                senhaDigitada = "";
                break;
              }
            }
          }
        }
        break;

      case 4://estado de alteração de  senha
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Alterar senha");
        lcd.setCursor(0,1);
        lcd.print("Senha: ");
      
        teclaLida = teclado.getKey();
      
        if(teclaLida) {
          if (contaDigitos < 4) {
            senhaDigitada = senhaDigitada + teclaLida;
            lcd.print("*");
            contaDigitos = contaDigitos + 1;
          }                                                 
          else {
            if (teclaLida == '#'){
              senhaAtual == senhaDigitada;
              contaDigitos = 0;
              senhaDigitada = "";
              estado = 0;
              break;              
            }
          }
        }
        break;
 
      default:
        estado = 3;
        break;
    
    }
  
}


void ligarAlarme() {
  //Ligar o led
  digitalWrite(LEDPIN, HIGH);
  //Ligar o buzzer com uma frequencia de 700 Hz.
  tone(BUZZERPIN,700);
  delay(2000); //tempo que o led fica ligado e o buzzer toca
  desligarAlarme();
}

void desligarAlarme() {
  //Desligar o led
  digitalWrite(LEDPIN, LOW);
  //Desligar o buzzer
  noTone(BUZZERPIN);
}
 
  //Leitura do PIR que pode ser 1 ou 0. 1 detecta movimento. 0 não detecta movimento.
//  estadoPIR = digitalRead(PIRPIN);
//  Serial.print("Valor do Sensor PIR: ");
//  Serial.println(estadoPIR);
 
  //Verificar se ocorreu detecção de movimentos  
//  if (estadoPIR == 1) {
//    ligarAlarme();
//  } else {
//     desligarAlarme();
//  }



