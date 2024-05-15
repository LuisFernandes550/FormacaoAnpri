#include <RTClib.h>//INCLUSÃO DA BIBLIOTECA para Relógio RTC
#include <Stepper.h>//biblioteca motor passo
#include <LiquidCrystal_I2C.h>//biblioteca para o LCD
#include <Wire.h> //INCLUSÃO DA BIBLIOTECA

//botões
const int btnPin = 7;
int modo, horas, botao;

//stepper
const int stepsPerRevolution=500;//revoluções exatas para abrikr tampa completa
Stepper myStepper= Stepper (stepsPerRevolution, 8, 10,9,11);

//LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Relógio
RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231

//DECLARAÇÃO DOS DIAS DA SEMANA

char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};
//variáveis
String datahora;
int dia, mes, ano, hora, min, datahoraAbertura;

void setup(){
  myStepper.setSpeed(9);
   lcd.backlight();
  Serial.begin(9600); //INICIALIZA A SERIAL
  if(! rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ
    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
    while(1); //SEMPRE ENTRE NO LOOP
  }
  if(rtc.lostPower()){ //SE RTC FOI LIGADO PELA PRIMEIRA VEZ / FICOU SEM ENERGIA / ESGOTOU A BATERIA, FAZ
    Serial.println("DS3231 OK!"); //IMPRIME O TEXTO NO MONITOR SERIAL
    //REMOVA O COMENTÁRIO DE UMA DAS LINHAS ABAIXO PARA INSERIR AS INFORMAÇÕES ATUALIZADAS EM SEU RTC
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
   // rtc.adjust(DateTime(2023, 6, 18, 12, 33, 00)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
  }
  pinMode(btnPin, INPUT_PULLUP);//com resitência por softwaree ligado ao gnd
 // pinMode(btnPin, INPUT);
  modo=0;//em 16 horas
 // rtc.adjust(DateTime(2023, 6, 21, 22, 58, 30));//acertar a hora se necessário
  horas = 16;
 botao=0; 
 //myStepper.step(-stepsPerRevolution);//linha de teste
}

void loop () {
  botao=digitalRead(btnPin);
  lcd.init();  
  DateTime now = rtc.now(); //CHAMADA DE FUNÇÃO
  dia = now.day();
  mes = now.month();
  ano = now.year();
  hora = now.hour();
  min = now.minute();
  lcd.setCursor(0,0);//linha, coluna
  datahora = datahora +dia+"/" +mes+"/" +ano+" " +hora+":" +min;
  lcd.print(datahora);
  if(modo==0){//condição inicial
    lcd.setCursor(0,1);
    lcd.print("Abre em 16h");
    datahoraAbertura = now.hour()+16;//ADICIONA O VALOR INICIAL 16 HORAS
    if(datahoraAbertura>=24){//Caso passe das 24 horas temos de as retirar para obter a hora certa
      datahoraAbertura-=24;
    }
    Serial.print(datahoraAbertura);
  }else if(modo==1 && datahoraAbertura!=25)//fora da condição inicial e ainda não abriu compartimento
  {
    lcd.setCursor(0,1);
    lcd.print("Abre em ");
    lcd.print(horas);
    lcd.print("h");
    
    Serial.print(datahoraAbertura);
    Serial.print("hora");
    Serial.print(hora);
  }else{
    lcd.setCursor(0,1);
    lcd.print("Abriu as ");
    lcd.print(hora);
    lcd.print("h");
  }
  if(botao==1){
    Serial.print("Botão: ");
    Serial.println(botao);
    modo=1;//abandona condição inicial
    horas-=1; 
    if(horas==0){
      horas=16;//dá a volta
    }
    datahoraAbertura = now.hour()+horas;//ADICIONA O VALOR INICIAL 16 HORAS
    if(datahoraAbertura>=24){//Caso passe das 24 horas temos de as retirar para obter a hora certa
      datahoraAbertura-=24;
    } 
    lcd.setCursor(0,1);
    lcd.print("Abre em ");
    lcd.print(horas);
    lcd.print("h"); 
  }
  if(datahoraAbertura==hora){//desta forma mal entre na hora ativa o motor passo, não tem em conta minutos ou segundos
    myStepper.step(-stepsPerRevolution);//abre o compartimento contra horário.
    datahoraAbertura=25;//valor nunca atingível para que o motor nunca mais rode de novo
  }
  datahora="";//reset datahora LCD
  //delay(1000); //INTERVALO DE 1 SEGUNDO
}
