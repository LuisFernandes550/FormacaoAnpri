#include <Keypad.h>
#include <LiquidCrystal.h>

//LiquidCrystal lcd(25, 27, 29, 31, 33, 35);
LiquidCrystal lcd(52, 50, 48, 46, 44, 42);

// seta para trás
byte darrow[8] = { 
  B00100, 
  B00100, 
  B00100, 
  B00100, 
  B00100,
  B11111, 
  B01110, 
  B00100 
};

// seta para a direita
byte rarrow[8] = {
  B00000,
  B00100,
  B00010,
  B11111,
  B00010,
  B00100,
  B00000,
  B00000
};

// seta para a esquerda
byte larrow[8] = {
  B00000,
  B00100,
  B01000,
  B11111,
  B01000,
  B00100,
  B00000,
  B00000
};

// seta em frente
byte uarrow[8] = {
  B00100,
  B01110,
  B11111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100
};

// pausa
byte pause[8] = {
  B01010,
  B01010,
  B01010,
  B01010,
  B01010,
  B01010,
  B01010,
  B01010
};

float wheel_dia=63; //    # mm (increase = spiral out)
float wheel_base=109; //    # mm (increase = spiral in, ccw) 
int steps_rev=512; //        # 512 for 64x gearbox, 128 for 16x gearbox
int delay_time=3  ; //         # time between steps in ms

// Stepper sequence org->pink->blue->yel
int R_stepper_pins[] = {2, 4, 3, 5};
int L_stepper_pins[] = {8, 10, 9, 11};

int fwd_mask[][4] =  {{1, 0, 1, 0},
                      {0, 1, 1, 0},
                      {0, 1, 0, 1},
                      {1, 0, 0, 1}};

int rev_mask[][4] =  {{1, 0, 0, 1},
                      {0, 1, 0, 1},
                      {0, 1, 1, 0},
                      {1, 0, 1, 0}};
                                           
int ledFoward = 24; // Led Pin luz da Frente
int ledLeft = 26; // Led Pin luz da Esquerda
int ledRight = 28; // Led Pin luz da Direita
int ledBackwards = 30; // Led Pin luz de Marcha-a-trás
int ledStop = 32; // Led Pin luz de STOP
int ledGO = 34; // Led Pin luz de GO

const byte ROWS = 3; //four rows
const byte COLS = 3; //four columns

// ARRAY
int lenth = 0;
int* myArray = 0;

//Resets Arduino
void(* resetFunc) (void) = 0;


char hexaKeys[ROWS][COLS] = {
{' ','2',' '},
{'4','5','6'},
{'#','7','0'},
};

byte rowPins[ROWS] = {
  35, 33, 31}; //portas de ligação das linhas do teclado
byte colPins[COLS] = {
 47, 45, 43}; //portas de ligação das colunas do teclado

//inicializa uma instancia da class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup(){
  Serial.begin(9600);
  
  lcd.createChar(0, darrow);
  lcd.createChar(1, larrow);
  lcd.createChar(2, rarrow);
  lcd.createChar(3, uarrow);
  lcd.createChar(4, pause);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.print("Programa-me");

  lcd.setCursor(0, 1);
  lcd.print("   ");
  lcd.write(byte(0));
  lcd.print(" ");
  lcd.write(byte(1));
  lcd.print(" ");
  lcd.write(byte(4));
  lcd.print(" ");
  lcd.write(byte(2));
  lcd.print(" ");
  lcd.write(byte(3));

  pinMode(ledFoward, OUTPUT);  
  pinMode(ledLeft, OUTPUT);
  pinMode(ledRight, OUTPUT);
  pinMode(ledBackwards, OUTPUT);
  pinMode(ledStop, INPUT);
  pinMode(ledGO, OUTPUT);
  
  myArray = (int*) malloc(lenth * sizeof(int));
  Serial.begin(9600);
 
  for(int pin=0; pin<4; pin++){
    pinMode(L_stepper_pins[pin], OUTPUT);
    digitalWrite(L_stepper_pins[pin], LOW);
    pinMode(R_stepper_pins[pin], OUTPUT);
    digitalWrite(R_stepper_pins[pin], LOW);
  } 
}
  
void loop(){
  char customKey = customKeypad.getKey();
 
  switch (customKey) {
    case '0':
      Serial.println("Pause");
      myArray = (int*) realloc(myArray, lenth * sizeof(int));
      myArray[lenth] = 0;
      lenth = lenth + 1;
      lcd.clear();
      lcd.print("Pausa");
      lcd.setCursor(0, 1);
      lcd.print("       ");
      lcd.write(byte(4));  
      break;

    case '2':
      Serial.println("Go Foward");
      myArray = (int*) realloc(myArray, lenth * sizeof(int));
      myArray[lenth]= 2;  
      lenth = lenth + 1;
      lcd.clear();
      lcd.print("Ir Frente");
      lcd.setCursor(0, 1);
      lcd.print("       ");
      lcd.write(byte(3));      
      break;

    case '4':
      Serial.println("Trun Left");
      myArray = (int*) realloc(myArray, lenth * sizeof(int));
      myArray[lenth]= 4;  
      lenth = lenth + 1;
      lcd.clear();
      lcd.print("Virar Esquerda");
      lcd.setCursor(0, 1);
      lcd.print("      ");
      lcd.write(byte(1)); 
      lcd.print("90");
      lcd.print((char)223);
      break;
      
    case '5':
      Serial.println("Go");
      lcd.clear();
      lcd.print("Go");
      digitalWrite(ledStop, LOW);
      digitalWrite(ledGO, HIGH);
      Go();
      digitalWrite(ledStop, HIGH);
      digitalWrite(ledGO, LOW);
      break;
      
    case '6':
      Serial.println("Trun Right");
      myArray = (int*) realloc(myArray, lenth * sizeof(int));
      myArray[lenth]= 6;  
      lenth = lenth + 1;
      lcd.clear();
      lcd.print("Virar Direita");
      lcd.setCursor(0, 1);
      lcd.print("      ");
      lcd.print("90");
      lcd.write(byte(2)); 
      lcd.print((char)223);
      break;

   case '7':
      Serial.println("Go Backwards");
      myArray = (int*) realloc(myArray, lenth * sizeof(int));
      myArray[lenth]= 7;  
      lenth = lenth + 1;
      lcd.clear();
      lcd.print("Ir Atras");
      lcd.setCursor(0, 1);
      lcd.print("       ");
      lcd.write(byte(0));
      break;

    case '#':
      Reset();    
      resetFunc();      
      break;
  }
  
}
void Go(){
  int i;
  for (i = 0; i < lenth; i = i + 1) {
    Serial.println(myArray[i]);
   switch (myArray[i]) {
    case 0:
      lcd.clear();
      lcd.print("A Espera");
      lcd.setCursor(0, 1);
      lcd.print("       ");
      lcd.write(byte(4));
      delay(1000);
      break;

    case 2:
      lcd.clear();
      lcd.print("Andar em Frente");
      lcd.setCursor(0, 1);
      lcd.print("       ");
      lcd.write(byte(3));
      digitalWrite(ledFoward, HIGH);
      forward(210);
      digitalWrite(ledFoward, LOW);
      delay(1000);
      break;
     
    case 4:
      lcd.clear();
      lcd.print("Virar Esquerda");
      lcd.setCursor(0, 1);
      lcd.print("      ");
      lcd.write(byte(1)); 
      lcd.print("90");
      lcd.print((char)223);
      digitalWrite(ledLeft, HIGH);
      left(129);
      delay(1000);
      digitalWrite(ledLeft, LOW);
      break;

    case 6:
      lcd.clear();
      lcd.print("Virar Direita");
      lcd.setCursor(0, 1);
      lcd.print("      ");
      lcd.print("90");
      lcd.write(byte(2)); 
      lcd.print((char)223);
      digitalWrite(ledRight, HIGH);
      right(129);
      delay(1000);
      digitalWrite(ledRight, LOW);
      break;
     
    case 7:
      lcd.clear();
      lcd.print("Andando Atras");
      lcd.setCursor(0, 1);
      lcd.print("       ");
      lcd.write(byte(0));
      digitalWrite(ledBackwards, HIGH);      
      backward(210);
      digitalWrite(ledBackwards, LOW);
      break;
    }   
  }
   lcd.clear();
   lcd.print("Terminei ;)");
}

// ----- HELPER FUNCTIONS -----------
int step(float distance){
  int steps = distance * steps_rev / (wheel_dia * 3.1412); //24.61
  return steps;  
}


void forward(float distance){
  int steps = step(distance);
  Serial.println(steps);
  for(int step=0; step<steps; step++){
    for(int mask=0; mask<4; mask++){
      for(int pin=0; pin<4; pin++){
        digitalWrite(L_stepper_pins[pin], rev_mask[mask][pin]);
        digitalWrite(R_stepper_pins[pin], fwd_mask[mask][pin]);
      }
      delay(delay_time);
    } 
  }
}

void backward(float distance){
  int steps = step(distance);
  for(int step=0; step<steps; step++){
    for(int mask=0; mask<4; mask++){
      for(int pin=0; pin<4; pin++){
        digitalWrite(L_stepper_pins[pin], fwd_mask[mask][pin]);
        digitalWrite(R_stepper_pins[pin], rev_mask[mask][pin]);
      }
      delay(delay_time);
    } 
  }
}

void right(float degrees){
  float rotation = degrees / 360.0;
  float distance = wheel_base * 3.1412 * rotation;
  int steps = step(distance);
  for(int step=0; step<steps; step++){
    for(int mask=0; mask<4; mask++){
      for(int pin=0; pin<4; pin++){
        digitalWrite(R_stepper_pins[pin], rev_mask[mask][pin]);
        digitalWrite(L_stepper_pins[pin], rev_mask[mask][pin]);
      }
      delay(delay_time);
    } 
  }   
}

void left(float degrees){
  float rotation = degrees / 360.0;
  float distance = wheel_base * 3.1412 * rotation;
  int steps = step(distance);
  for(int step=0; step<steps; step++){
    for(int mask=0; mask<4; mask++){
      for(int pin=0; pin<4; pin++){
        digitalWrite(R_stepper_pins[pin], fwd_mask[mask][pin]);
        digitalWrite(L_stepper_pins[pin], fwd_mask[mask][pin]);
      }
      delay(delay_time);
    } 
  }   
}

void Reset(){
  //digitalWrite(ledred, LOW);
  lcd.clear();
  lcd.print("Programa-me");
}
