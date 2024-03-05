#define led 9 
#define micro 12

bool valor=false;

// Variaveis para ajudar na captação do bater de palmas
bool estagio1=false;
bool estagio2=false;
bool estagio3=false;

// Variaveis para fazer a função do tempo limite da onda em alta e baixa
int a;
int b;

void setup() {
  pinMode(led, OUTPUT); //Configura LED como saída
  pinMode(micro, INPUT); //Configura o microfone como entrada
 
  digitalWrite(led, LOW); //Começa com o LED apagado
}

void loop() {
  valor = digitalRead(micro);//Le o pino digital
 
  // Teste do primeiro bater de palmas
  if(valor==true && estagio1 == false){ //Primeiro testa se o pino esta em alta
    estagio1=true; //Ativa uma variavel auxiliar
    a=0; //Volta o temporizador para 0
  }
 
  // Agora testa se o pino que estava em alta abaixou
  if(valor==false && estagio1 == true && estagio2==false){
    estagio2=true; //Ativa a segunda variavel auxiliar
    a=0;
  }
 
  // Testa se o tempo em alta da primeira onda ultrapassou 200ms
  if(estagio1==true && estagio2==false){
    // Essa parte adiciona + 1 à variavel 'a' enquanto a onda esta em nivel alto; se a onda continuar em nivel alto por 200ms, o programa volta pro inicio
    delay(1);
    a++;
    if(a>200){
      estagio1=false; //Manda o programa voltar pra etapa 0
    }
  }

  // Testa se o tempo em alta da primeira onda ultrapassou 700ms
  if(estagio2==true){
    // Essa parte adiciona + 1 a variavel 'a' enquanto a onda esta em nivel baixo (etapa 2 true e etapa 3 false); se a onda continuar em nivel baixo por 200ms, o programa volta pro inciio
    if(estagio3 == false){
      delay(1);
      a++;
      if(a>700){
        estagio1=false; //Manda o programa voltar pra etapa 0
        estagio2=false;
      }
    }
 
    // Se a variavel 'a' chegar no minimo a 70ms (tempo em baixa) o programa pode começar a verificação da segunda onda
    // Esse tempo de 70us serve para o programa nao confundir vibrações que ocorrem em intervalos de tempo muito pequenos
    if(a>70){
      //Teste do segundo bater de palmas
      if(valor==true && estagio2 == true && estagio3 == false){ //Testa se o pino foi pra alta (observe que o estagio2== true é redundante, mas vou manter para facilitar o entendimento)
        estagio3=true; //Ativa a terceira variavel auxiliar
        b=0;//Volta o temporizador b para 0
      }

      // Agora testa se o pino que estava em alta abaixou (neste caso testa se ocorreu o bater de palmas duas vezes
      if(valor==false && estagio3 == true){
        digitalWrite(led, !digitalRead(led));//Inverte o sinal do LED
        estagio1=false; //Volta o programa voltar pra etapa 0
        estagio2=false;
        estagio3=false;
        //delay(1000); //Cria um delay ate a proxima amostragem para nao misturar os dados
      }

      // Testa se o tempo em alta da segunda onda ultrapassou 200ms
      if(estagio3==true){
        // Esta parte adiciona + 1 a variavel 'b' enquanto a onda (segunda) esta em nivel alto; se a onda continuar em nivel alto por 200ms, o programa volta pro inicio
        delay(1);
        b++;
        if(b>200){
          estagio1=false; // Se sim, manda o programa voltar pra etapa 0
          estagio2=false;
          estagio3=false;
        }
      }
    }
  }

  delayMicroseconds(1/44100*1000000);//Delay de 44Khz (frequencia comum para gravação de audio)
}
