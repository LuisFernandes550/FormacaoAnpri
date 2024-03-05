#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16,2);
int ldr1 = A0;//definindo o primeiro ldr 
int ldr2 = A1;//definindo o segundo ldr 
int ledred = 13;//definir o led vermelho na porta 13
int ledgreen = 12;//definir o led verde na porta 12
int valorldr1 = 0; //criando variavel para receber o valor do primeiro ldr
int valorldr2 = 0; //criando variavel para receber o valor do segundo ldr
Servo myservo;  // create servo object to control a servo


void setup() {
    lcd.init();
    myservo.attach(2);//definindo o servo na porta digital 2
    myservo.write(140); //posição inicial do motor (graus)
    pinMode(ldr1,INPUT); //definindo o ldr como componente de entrada
    pinMode(ldr2,INPUT);  //definindo o ldr como componente de entrada 
    pinMode(ledred, OUTPUT);  
    pinMode(ledgreen, OUTPUT);  
    Serial.begin(9600); //iniciando a comunicação serial

}

void loop() {
    int valor1 = analogRead(ldr1);
    int valor2 = analogRead(ldr2);//fazendo as variaveis receberem os valores capitados pelos sensores
     Serial.print(" Valor do primeiro sensor:");
     Serial.print(valorldr1);
     delay(2000);
     Serial.print(" Valor do segundo sensor:");
     Serial.println(valorldr2);

   if(valorldr1<900){ //quando a luminosidade for menor que 700 o motor levantará um angulo de 90 graus (simulando um portão)
   digitalWrite(ledred, HIGH);//liga led vermelho
    digitalWrite(ledgreen, LOW);//desliga led verde
    myservo.write(90);//'abrindo o portão'
    delay(2000);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print ("Cancela Aberta!");
    digitalWrite(ledgreen, HIGH);//liga led verde
    digitalWrite(ledred, LOW);//desliga led vermelho
    delay(8000);
    }

   if(valorldr2<900){ //O mesmo se repete no 2ºldr, porém a estrutura é feita para o fecho do portão
    myservo.write(140);//'fechando o portão'
    delay(2000);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print ("Cancela Fechada!");
    digitalWrite(ledred, HIGH);//liga led vermelho
    digitalWrite(ledgreen, LOW);//desliga led verde
    delay(8000);
  }
}
