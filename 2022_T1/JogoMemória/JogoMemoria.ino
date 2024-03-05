//Definir as notas para os sons
#define NOTE_D4  294
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_A5  880
 
//Criar o array para os 4 sons para sortear um som.
int tons[4] = { NOTE_A5, NOTE_A4, NOTE_G4, NOTE_D4 };
//A sequência de até 25 itens vai começar vazia.
int sequencia[25] = {};
//Indica a rodada atual que o jogo se encontra.
int rodada_atual = 0;
//Indica o passo atual dentro da sequência.
int passo_atual_na_sequencia = 0;
 
/*
 *Indicar os pinos de áudio, leds e botões.
 */
int pinoAudio = 12;
int pinosLeds[4] = { 2, 4, 6, 8 };
int pinosBotoes[4] = { 3, 5, 7, 9 };
 
// Indicar se um botão foi pressionado durante o loop principal.
int botao_pressionado = 0;
// Indicar se o jogo acabou.
int perdeu_o_jogo = false;
 
void setup() {
  // Definir o modo dos pinos dos Leds como saída.
  for (int i = 0; i <= 3; i++) {
    pinMode(pinosLeds[i], OUTPUT);
  }
 
  // Definir o modo dos pinos dos Botões como pullup interno.
  for (int i = 0; i <= 3; i++) {
    pinMode(pinosBotoes[i], INPUT_PULLUP);
  }
 
  // Definir o modo do pino de Áudio como saída.
  pinMode(pinoAudio, OUTPUT);
 
  // Iniciar o random através de uma leitura da porta analógica.
  // Esta leitura gera um valor varivariável entre 0 e 1023.
  randomSeed(analogRead(0));
}
 
void loop() {
  // Se perdeu o jogo reiniciamos todas as variáveis.
  if (perdeu_o_jogo) {
    int sequencia[25] = {};
    rodada_atual = 0;
    passo_atual_na_sequencia = 0;
    perdeu_o_jogo = false;
  }
 
  // Toca um som de início para anunciar que o jogo está a começar quando é a primeira rodada.
  if (rodada_atual == 0) {
    tocarSomDeInicio();
    delay(500);
  }

// Chama a função que inicia a próxima rodada.
  proximaRodada();
  // Reproduz a sequência atual.
  reproduzirSequencia();
  // Aguarda os botões serem pressionados pelo jogador.
  aguardarJogador();
 
  // Aguarda 1 segundo entre cada jogada.
  delay(1000);
}
 
// Sorteia um novo item e adiciona na sequência.
void proximaRodada() {
  int numero_sorteado = random(0, 4);
  sequencia[rodada_atual++] = numero_sorteado;
}
 
// Reproduz a sequência para ser memorizada.
void reproduzirSequencia() {
  for (int i = 0; i < rodada_atual; i++) {
    tone(pinoAudio, tons[sequencia[i]]);
    digitalWrite(pinosLeds[sequencia[i]], HIGH);
    delay(500);
    noTone(pinoAudio);
    digitalWrite(pinosLeds[sequencia[i]], LOW);
    delay(100);
  }
  noTone(pinoAudio);
}
 
// Aguarda o jogador iniciar sua jogada.
void aguardarJogador() {
  for (int i = 0; i < rodada_atual; i++) {
    aguardarJogada();
    
  // verifica a jogada  
  if (sequencia[passo_atual_na_sequencia] != botao_pressionado) {
      gameOver(); // perdeu
   }
   
   // pára o jogo se perdeu
    if (perdeu_o_jogo) {
      break;
    }
     passo_atual_na_sequencia++;
  }
 
  // Redefine a variável para 0.
  passo_atual_na_sequencia = 0;
}
 
void aguardarJogada() {
  boolean jogada_efetuada = false;
  while (!jogada_efetuada) {
    for (int i = 0; i <= 3; i++) {
      if (!digitalRead(pinosBotoes[i])) {
        // Dizendo qual foi o botao pressionado.
        botao_pressionado = i;
 
        tone(pinoAudio, tons[i]);
        digitalWrite(pinosLeds[i], HIGH);
        delay(300);
        digitalWrite(pinosLeds[i], LOW);
        noTone(pinoAudio);
 
        jogada_efetuada = true;
      }
    }
    delay(10);
  }
}
 
void gameOver() {
    // GAME OVER.
    for (int i = 0; i <= 3; i++) {
      tone(pinoAudio, tons[i]);
      digitalWrite(pinosLeds[i], HIGH);
      delay(200);
      digitalWrite(pinosLeds[i], LOW);
      noTone(pinoAudio);
    }
 
    tone(pinoAudio, tons[3]);
    for (int i = 0; i <= 3; i++) {
      digitalWrite(pinosLeds[0], HIGH);
      digitalWrite(pinosLeds[1], HIGH);
      digitalWrite(pinosLeds[2], HIGH);
      digitalWrite(pinosLeds[3], HIGH);
      delay(100);
      digitalWrite(pinosLeds[0], LOW);
      digitalWrite(pinosLeds[1], LOW);
      digitalWrite(pinosLeds[2], LOW);
      digitalWrite(pinosLeds[3], LOW);
      delay(100);
    }
    noTone(pinoAudio);
 
    perdeu_o_jogo = true;  
} 
 
void tocarSomDeInicio() {
  tone(pinoAudio, tons[0]);
  digitalWrite(pinosLeds[0], HIGH);
  digitalWrite(pinosLeds[1], HIGH);
  digitalWrite(pinosLeds[2], HIGH);
  digitalWrite(pinosLeds[3], HIGH);
  delay(500);
  digitalWrite(pinosLeds[0], LOW);
  digitalWrite(pinosLeds[1], LOW);
  digitalWrite(pinosLeds[2], LOW);
  digitalWrite(pinosLeds[3], LOW);
  delay(500);
  noTone(pinoAudio);
}