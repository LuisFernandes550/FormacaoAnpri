/*
	Durante a noite debaixo da cama acende uma fita de led (3 metros) quando deteta movimento 
  enquanto está escuro.
	O circuito:
	* PIR para o movimento. A configuração dos sensores depende da cama. 
	* LDR para sensor de luz
        * Transistor para conduzir a faixa LED
*/
#define ldr_pin A5
#define led_pin 9

const int numberOfPirs = 1;              // Número do sensor PIR
const int pir_pins[numberOfPirs] = {8};  // Pins para o pir sensor Exempl0 = {pin,pin,pin}

const int led_time = 10; // Tempo para os leds ligados em segundos
const int on_delay = 20; // Delay para os leds ligados (em mSec)
const int off_delay = 1; // Delay para os leds desligados (em mSec)

const int ldr_value = 500; // Valor da diferença entre escuro e claro

const int led_max = 180;  // valor máximo para a Ledstrip em pwm
const int led_min = 0;    // valor minimo em pwm (0 recomendado, senão não vai desligar)

boolean licht = false;   // Boolean, é verdade quando a luz está acesa;
boolean led = false;     // Boolean, é verdade quando a fita de led está acesa;
int timer = 0;           // Integer para a variável timer;

void setup(){

  pinMode(ldr_pin,INPUT);
  
  // Define o modo pino para todos os sensores pir
  for (int i=0; i < numberOfPirs; i++){
    pinMode(pir_pins[i],INPUT);
  }
  
  pinMode(led_pin,OUTPUT);
}

void loop(){
  
  // Lê o valor do ldr 
  int ldr = analogRead(ldr_pin);
 
  // Ler todos os sensores pir
  boolean pir = false;    
  for (int i=0; i < numberOfPirs; i++){
      if (digitalRead(pir_pins[i]) == 1){
        pir = true;    
      }
  }

  // verifica se o quarto tem luz suficiente
  if (ldr > ldr_value){
    licht = false; 
  }else{
    licht = true; 
  }
 
  // Se o quarto estiver escuro e for detetado movimento, os leds acendem e o timer passa a 0
  if (pir == true && licht == false){
    ledAan();
    timer = 0;
  }
  
  // Se os LEDs estiverem acesos e a sala tiver luz suficiente, os LEDs desligam
  if (led == true && licht == true){
    ledUit(); 
  }
  
  // Se os leds estiverem ligados é incrementado o timer
  if (led == true) timer++;
  
  // Se o temporizador for igual ao tempo definido, desligam os LEDs.
  if (timer == led_time){
    ledUit(); 
  }
 
  delay(1000);
}

// função para os leds ligados
void ledAan(){
  if (led == false){
    for(int i=led_min;i<=led_max; i++){
      analogWrite(led_pin,i);
      delay(on_delay);
    }    
    led = true;
  }
}

// função para desligar os leds
void ledUit(){
  if (led == true){
    for(int i=led_max;i>=led_min; i--){
      analogWrite(led_pin,i);
      delay(off_delay);      
    }    
    led = false;
  }
}
