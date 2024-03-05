//#include <SparkFun_APDS9960.h>
#include <Arduino_APDS9960.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>

int max_lugares = 2;            // máximo de participantes
int lugares_livres = 2;         // valor inicial de lugares livres
const int ledRed = 8;           // semáforo para entrada, vermelho - não entrar
const int ledGreen = 4;         // semáforo para entrada, verde - entrar
bool presenca = false;          // pessoas para entrarem
int const TRIGGER_PIN = 11;     // pino para o sinal de envio do sensor de ultrasom
int const ECHO_PIN = 12;        // pino para o sinal de retorno do sensor de ultrasom
long tempo;                     // tempo para o retorno do sinal ao sensor ultrasom
float distancia;                // distância entre o sensor e as pessoas a aguardarem entrada
const int presenca_cor_R = 10;  // Componente vermelha da luz de presença
const int presenca_cor_G = 9;   // Componente verde da luz de presença
const int presenca_cor_B = 5;   // Componente azul da luz de presença
const int lampada = 7;          // ativar a luz da sala

Ultrasonic ultrasonic(TRIGGER_PIN, ECHO_PIN);  //sensor de presença
LiquidCrystal_I2C lcd1(0x27, 16, 2);           // LCD à entrada da sala >LCD1
LiquidCrystal_I2C lcd2(0x27, 16, 2);           // LCD no interior da sala >LCD2
byte caracter_especial1[8] = { 0b00000, 0b01010, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000, 0b00000 };


/* seleção do canal pretendido*/
void TCA9548A(uint8_t bus) {
  Wire.beginTransmission(0x70);  // endereço do TCA9548A: 0x70
  Wire.write(1 << bus);          // envio do byte respetivo ao bus seleccionado
  Wire.endTransmission();
}

/* setup do LCD1 */
void iniciar_lcd1() {
  TCA9548A(1);
  lcd1.init();
  lcd1.backlight();
  lcd1.setCursor(2, 0);
  lcd1.print("BEM-VINDOS!!");
  lcd1.setCursor(0, 1);
  lcd1.print("Evento");
  lcd1.createChar(0, caracter_especial1);  // criar um carater especial
  for (int i = 0; i < 3; i++) {
    lcd1.setCursor(8 + i, 1);
    lcd1.write((byte)0);
  }
}
/* setup do LCD2 */
void iniciar_lcd2() {
  TCA9548A(2);
  lcd2.init();
  lcd2.backlight();
  lcd2.setCursor(2, 0);
  lcd2.print("Porta Aberta");
  lcd2.setCursor(0, 1);
  lcd2.print("Participantes: ");
  lcd2.setCursor(15, 1);
  lcd2.print(max_lugares - lugares_livres);
}
/* setup do sensor de entradas */
void verificar_sensor_movimento() {
  TCA9548A(0);
  if (!APDS.begin()) {
    Serial.println("Error initializing APDS-9960 sensor!");
  }
}
void detectar_entradas_saidas() {
  TCA9548A(0);
  if (APDS.gestureAvailable()) {
    // a gesture was detected, read and print to Serial Monitor
    Serial.println("Movimento detectado");
  }
  int movimento = APDS.readGesture();

  switch (movimento) {
    case GESTURE_UP:
      Serial.println("Movimento para cima");
      break;
    case GESTURE_DOWN:
      Serial.println("Movimento para baixo");
      break;
    case GESTURE_LEFT:
      Serial.println("Detectada Saída");

      if (lugares_livres < max_lugares) {
        lugares_livres += 1;
      }
      if (lugares_livres == max_lugares) {
        apagar_lampada_sala();
      }
      informar_LCD_externo_entrar(lugares_livres);
      informar1_LCD_interno(lugares_livres);
      digitalWrite(ledGreen, HIGH);
      digitalWrite(ledRed, LOW);
      break;
    case GESTURE_RIGHT:
      Serial.println("Detectada Entrada");
      ligar_lampada_sala();
      if (lugares_livres > 1) {
        lugares_livres -= 1;
        
        informar_LCD_externo_entrar(lugares_livres);
        digitalWrite(ledGreen, HIGH);
        digitalWrite(ledRed, LOW);
      } else {
        if (lugares_livres == 1) {
          lugares_livres -= 1;
          //apagar_lampada_sala();
        }
        informar_LCD_externo_stop();
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledRed, HIGH);
      }
      informar1_LCD_interno(lugares_livres);
      break;
    default:
      // sem atribuição
      break;
  }
}
void detectar_presenca_entrada() {
  tempo = ultrasonic.timing();
  distancia = ultrasonic.convert(tempo, Ultrasonic::CM);
  Serial.print("Pessoas: ");
  Serial.println(distancia);
  if (distancia < 15) {
    presenca = true;
    acender_luz_presenca();
  } else {
    presenca = false;
    apagar_luz_presenca();
  }
}

void informar_LCD_externo_entrar(int lugares) {
  TCA9548A(1);
  lcd1.setCursor(2, 0);
  lcd1.print("BEM-VINDOS!!");
  lcd1.setCursor(0, 1);
  lcd1.print("Lugares Livres");
  lcd1.setCursor(15, 1);
  lcd1.print(lugares);
}
void informar_LCD_externo_stop() {
  TCA9548A(1);
  lcd1.clear();
  lcd1.setCursor(0, 0);
  lcd1.print(" **SALA CHEIA**  ");
  lcd1.setCursor(0, 1);
  lcd1.print(" Aguardar sff");
}
/* informação do nr de participantes na sala e existência de pessoas à espera*/
void informar1_LCD_interno(int lugares) {
  TCA9548A(2);
  lcd2.clear();
  lcd2.setCursor(0, 0);
  lcd2.print("Participantes:");
  lcd2.setCursor(15, 0);
  lcd2.print(max_lugares - lugares);
  lcd2.setCursor(0, 1);
  lcd2.print("Espera:");
  lcd2.setCursor(7, 1);
  if (presenca == true) {
    lcd2.print("SIM");
  } else {
    lcd2.print("NAO");
  }
}
void acender_luz_presenca() {
  analogWrite(presenca_cor_R, 128);
  analogWrite(presenca_cor_G, 50);
  analogWrite(presenca_cor_B, 50);
}
void apagar_luz_presenca() {
  analogWrite(presenca_cor_R, 0);
  analogWrite(presenca_cor_G, 0);
  analogWrite(presenca_cor_B, 0);
}
void ligar_lampada_sala() {
  digitalWrite(lampada, HIGH);
}
void apagar_lampada_sala() {
  digitalWrite(lampada, LOW);
}
void setup() {
  Serial.begin(9600);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(presenca_cor_R, OUTPUT);
  pinMode(presenca_cor_G, OUTPUT);
  pinMode(presenca_cor_B, OUTPUT);
  pinMode(lampada, OUTPUT);


  while (!Serial)
    ;

  Serial.println("Início");

  Wire.begin();
  iniciar_lcd1();
  iniciar_lcd2();
  verificar_sensor_movimento();
}

void loop() {

  detectar_presenca_entrada();
  informar1_LCD_interno(lugares_livres);
  detectar_entradas_saidas();

}