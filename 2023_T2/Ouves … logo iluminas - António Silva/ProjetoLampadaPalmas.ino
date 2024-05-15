//*************************************************************** março de 2024
// Código do projeto da Formação - Programação com Arduinos - António Silva
//Definir os pinos para o microfone e relé
int relay = 8;       // Porta digital onde está ligado relé
int microphone = A0; // porta analógica onde está ligado o microfone analógico
int ledPin = 8;
int releLigado = 0;
int contador = 0;  //Contador para detetar palmas palmas

// Após calibar as palmas o seu som dura cerca de 160 ms
const int valorIntermedioDuracaoPalma = 120; // Valor intermédio para detetar Palma
// Valor de repouso superior à duração da Palma para detetar ausência de Palma
const int repouso = 500;
 
int leituraSom; // Para guardar a leitura do microfone
unsigned long tempoDecorrido;
const int LimiteSom = 100; // A definição do valor desta constante depende da calibração do microfone e de diversas leituras enviadas para a porta serie 

void setup() {
  // Serial.begin(9600);  
  // Configura os modos dos pinos
  pinMode(relay,OUTPUT);
  pinMode(microphone, INPUT);
  // desliga o Relé
  digitalWrite(relay, LOW);
  releLigado = 0;
  // Inicia a contagem de tempo 
  tempoDecorrido = millis();
}

void loop() {
  leituraSom = analogRead(microphone);
  if (leituraSom > LimiteSom) {
      if (!contador) {// é a primeira vez que deteta som acima do limite de som
        contador++; // incrementa o contador para esperar tempo definido para detetar palma
      }
      else { // O contador está > 0 vê se já passou o tempo para considerar o valor válido 
        if ((millis() - tempoDecorrido) > valorIntermedioDuracaoPalma) { // palma detetada ativa ou desativa o relé
          if (releLigado) {
              Serial.println("Desliga RELÉ"); //  Apenas usado em debug
              digitalWrite (relay,LOW);
              releLigado = 0;
          }
          else {
              Serial.println("Liga RELÉ");
              digitalWrite (relay,HIGH);
              releLigado = 1;
           }
           contador = 0; // reinicia o contador
           tempoDecorrido = millis(); // reinicia a contagem de tempo
        }     
      }
  }
  else{ // vê se já passou demasiado tempo para detetar a palma
    if ((millis() - tempoDecorrido) > repouso) {
      contador = 0;
      tempoDecorrido = millis();
    }
  }  
}
    
  
