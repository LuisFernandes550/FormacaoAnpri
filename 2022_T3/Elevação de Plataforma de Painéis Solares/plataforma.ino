#include <Stepper.h> //INCLUSÃO DE BIBLIOTECA
#include <Wire.h>
#include <Wire.h> //INCLUSÃO DA BIBLIOTECA
#include "RTClib.h" //INCLUSÃO DA BIBLIOTECA

RTC_DS3231 rtc; //OBJETO DO TIPO RTC_DS3231

//DECLARAÇÃO DOS DIAS DA SEMANA
char daysOfTheWeek[7][12] = {"Domingo", "Segunda", "Terça", "Quarta", "Quinta", "Sexta", "Sábado"};
 


const int stepsPerRevolution = 12; //NÚMERO DE PASSOS POR VOLTA
int valor=0; int hora=0; int minuto=0;

Stepper myStepper1(stepsPerRevolution, 4,6,5,7); //INICIALIZA O MOTOR UTILIZANDO OS PINOS DIGITAIS 4, 5, 6, 7  motor1 cabeça
Stepper myStepper2(stepsPerRevolution, 8,10,9,11); //INICIALIZA O MOTOR UTILIZANDO OS PINOS DIGITAIS 8, 9, 10, 11  motor2 lado

void setup(){
  myStepper1.setSpeed(300); //VELOCIDADE DO MOTOR1
  myStepper2.setSpeed(300); //VELOCIDADE DO MOTOR2
  Serial.begin(9600); //INICIALIZA A SERIAL
  if(! rtc.begin()) { // SE O RTC NÃO FOR INICIALIZADO, FAZ
    Serial.println("DS3231 não encontrado"); //IMPRIME O TEXTO NO MONITOR SERIAL
    while(1); //SEMPRE ENTRE NO LOOP
  }
  if(rtc.lostPower()){ //SE RTC FOI LIGADO PELA PRIMEIRA VEZ / FICOU SEM ENERGIA / ESGOTOU A BATERIA, FAZ
    Serial.println("DS3231 OK!"); //IMPRIME O TEXTO NO MONITOR SERIAL
    //REMOVER O COMENTÁRIO DE UMA DAS LINHAS ABAIXO PARA INSERIR AS INFORMAÇÕES ATUALIZADAS EM SEU RTC
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //CAPTURA A DATA E HORA EM QUE O SKETCH É COMPILADO
    //rtc.adjust(DateTime(2018, 9, 29, 15, 00, 45)); //(ANO), (MÊS), (DIA), (HORA), (MINUTOS), (SEGUNDOS)
  }
  delay(100); //INTERVALO DE 100 MILISSEGUNDOS
}

void loop(){
DateTime now = rtc.now();
//hora = now.hour();
minuto = now.minute();
 //Serial.print(now.hour(), DEC); //IMPRIME NO MONITOR SERIAL A HORA
 Serial.println(minuto); //IMPRIME NO MONITOR SERIAL A HORA
  switch (minuto) {
  case (8):
    // eleva a plataforma
      
    while (valor == 0) {
      Serial.println("eleva");
        //LAÇO "for" QUE LIMITA O TEMPO EM QUE O MOTOR GIRA NO SENTIDO HORÁRIO            
     for(int i = 0; i < 50; i++){//PARA "i" IGUAL A 0, ENQUANTO "i" MENOR QUE 50 INCREMENTA "i"
           myStepper2.step(stepsPerRevolution);
      }
     for(int i = 0; i < 50; i++){//PARA "i" IGUAL A 0, ENQUANTO "i" MENOR QUE 50 INCREMENTA "i"
           myStepper1.step(stepsPerRevolution);
      }
      valor=1;
      hora = now.hour();
    }
    break;
  case (9):
     
         //ao fim de uma hora inclina menos, baixa no sentido poente para ter mais incidencia de luz
    while (valor == 1) {
       Serial.println("baixa motor 2 lateral");
    
     for(int i = 0; i < 20; i++){ //PARA "i" IGUAL A 0, ENQUANTO "i" MENOR QUE 50 INCREMENTA "i"
          myStepper2.step(-stepsPerRevolution); //GIRA O MOTOR NO SENTIDO ANTI-HORÁRIO
     }
      valor=0;
      hora = now.hour();
     }
      case (10):
     
         //retorna a plataforma à posição inicial
    while (valor == 0) {
       Serial.println("baixa");
       for(int i = 0; i < 50; i++){ //PARA "i" IGUAL A 0, ENQUANTO "i" MENOR QUE 50 INCREMENTA "i"
          myStepper1.step(-stepsPerRevolution); //GIRA O MOTOR NO SENTIDO ANTI-HORÁRIO
       }
       for(int i = 0; i < 30; i++){ 
          myStepper2.step(-stepsPerRevolution); 
      valor=1;
      hora = now.hour();
     }
    }
    break;
 
  }
}