//**** Sensor de Humidade e temperatura ***********
#include "DHT.h"
#include<Wire.h>
#include<LiquidCrystal_I2C.h>

//inicializa o display
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define DHTPIN 12 //define a variável constante relativa ao pino digital do sensor DHT11
#define DHTTYPE DHT11//define o tipo de sensor utilizado
DHT dht(DHTPIN,DHTTYPE);//cria o objecto do tipo DHT

float humidade, temperatura;

//**** Sensor de luz ***********
#define ledLuz 11
#define ldrPin A0
int ldr;

//**** Sensor de som ***********
//int somDigital ;
int somAnalogico; //A2
int ledSom =4; 

//**** Sensor de gás *********** 
int pinBuzzer=1;
int pinGas =A3;//pino usado pelo sensor de gas MQ-2
int maxGas=100; //define um limite para o nível normal de gás



void setup() {
  Serial.begin(9600);
  
  //*************** Inicia LCD ***********
  lcd.init(); 
  lcd.backlight();// acende a luz de fundo
  
  //*************** Sensor de Humidade e temperatura ***********
  dht.begin(); //inicia HHT
    
  //*************** Sensor de luz ***********
  pinMode (ledLuz,OUTPUT);
  pinMode (ldrPin,INPUT);

  //*************** Sensor de som ***********
  //pinMode(somDigital, INPUT);
  pinMode(somAnalogico, INPUT);

  //*************** Sensor de Gás *************
  pinMode(pinBuzzer, OUTPUT);  
  pinMode(pinGas, INPUT);
  
}

void loop() {
  //************************** leitura do sensor de Humidade e temperatura *********************************
  delay(2000);
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  humidade = dht.readHumidity();
  temperatura = dht.readTemperature(); // lê temperatura em Celsius
  
  // Print Tem e Humidade no LCD.
  lcd.clear(); //limpa o LCD
  lcd.setCursor(0,0);
  lcd.print("Humidade: "); 
  lcd.print(humidade);  
  lcd.print("%");    
  lcd.setCursor(0,1);
  lcd.print("Temp: ");
  lcd.print(temperatura);  
  lcd.print((char) 223);
  lcd.print("C"); 
  delay(4000);

  // Print Temp e Humidade no Serial
  Serial.print("Temperatura= ");
  Serial.println(temperatura);
  Serial.print("Humidade= ");
  Serial.println(humidade);
  Serial.println("  ");
  
  //************************** leitura do sensor de luz *********************************
  ldr=analogRead(ldrPin);
  Serial.print("Luz= ");
  Serial.println(ldr);
  // Print Luz no LCD.
  lcd.clear(); //limpa o LCD
  lcd.backlight();                 // acende a luz de fundo
  lcd.setCursor(0,0);
  lcd.print("Luz: "); 
  lcd.print(ldr); 
  
  if (ldr<300){
     digitalWrite(ledLuz, HIGH);
     Serial.println("Noite");
     lcd.setCursor(0,1);
     lcd.print("Noite ");
     }else {
      digitalWrite(ledLuz,LOW);
      Serial.println("Dia");
      lcd.setCursor(0,1);
      lcd.print("Dia ");      
     }
  Serial.println("  "); 
 
  delay(4000);

  //************************** leitura do sensor de som *********************************  
  somAnalogico=analogRead(A2);

  //traça o gráfico
  long sum =0;
  for (int i=0; i<100;i++){
    sum +=analogRead(somAnalogico);
  }
  sum=sum/100;
  
  Serial.print("Som Analogico:" );
  Serial.println(sum);
  
  //mostra o valor digital na primeira linha do LCD
  lcd.clear(); //limpa o LCD
  if(sum > 500){
    digitalWrite(ledSom, HIGH);
    lcd.setCursor(0,0);
    lcd.print("Som: ");
    lcd.print(sum);
    lcd.setCursor(0,1);
    lcd.print("Elevado    ");
    lcd.setCursor(0,1); 
    Serial.println("Elevado");

  }else{
    digitalWrite(ledSom, LOW);    
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Som: ");
    lcd.print(sum); 
    lcd.setCursor(0,1);
    lcd.print("Normal    ");
    lcd.setCursor(0,1);
    Serial.println("Normal");
  }
    Serial.println("  ");
    delay(4000);

  //************************** Leitura do Sensor de nível ********************************* 
  int nivel = analogRead(A1);
  lcd.clear(); //limpa o LCD
  lcd.setCursor(0, 0);
  lcd.print("Agua: ");
  lcd.print(nivel);
  lcd.print("   ");
  Serial.print("Nível de água: ");
  Serial.println(nivel);
  Serial.println("  ");

  lcd.setCursor(0,1);
  lcd.print("Nivel: ");

  if(nivel==0){
    lcd.print ("Cheio   ");
  }else if (nivel >1 && nivel<220) {
    lcd.print("Baixo   " );
  }else if (nivel> 220 && nivel <260) {
    lcd.print("Medio   " );
  }else if (nivel > 260 && nivel <270){
    lcd.print("Alto    " );
  }else if (nivel > 270){
    lcd.print("Critico    " );
  }

delay(4000);

// ************************** Leitura do Sensor de gás ********************************* 
  int valor_analogico =analogRead(pinGas);
  digitalWrite(pinBuzzer, LOW);
  Serial.print("Gás: ");
  Serial.println(valor_analogico);
  if(valor_analogico > maxGas){
    digitalWrite(pinBuzzer, HIGH);
    Serial.println("Nível crítico");
  }else{
    digitalWrite(pinBuzzer, LOW);
  }

  lcd.clear(); //limpa o LCD
  lcd.setCursor(0, 0);
  lcd.print("Gas: ");
  lcd.print(valor_analogico);
  lcd.print("       ");

  lcd.setCursor(0,1);
  if(valor_analogico > maxGas){
    lcd.print("Valor elevado!");
  }else  {
    lcd.print("                ");
  }
  Serial.println("  ");
  delay(100); 
}
