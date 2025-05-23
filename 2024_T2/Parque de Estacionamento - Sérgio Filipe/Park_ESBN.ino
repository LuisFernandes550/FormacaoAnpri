// Parque de Estacionamento ESBN Criado por Fernando Filipe 

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h> 

#define E 6  // Porta do Sensor E18_D80NK
#define Ir 7  // Porta do Sensor de proximidade IR
#define Led_Vermelho 8  // Porta do Led Vermelho
#define Led_Verde 10  // Porta Led Verde
int pos;
LiquidCrystal_I2C lcd(0x27,16,2);   
Servo mServo;

int Lugares_Estacionamento = 4;  // Número total de vagas do Parque de Estacionamento
int flag1 = 0;
int flag2 = 0;

void setup() {
    Serial.begin(9600); 
    lcd.init(); // Inicializa o LCD
    lcd.backlight(); // Liga a luz de fundo

    pinMode(Led_Vermelho, OUTPUT);
    pinMode(Led_Verde, OUTPUT);
    digitalWrite(Led_Verde, HIGH);
    digitalWrite(Led_Vermelho, LOW);    
    mServo.attach(9); // Conectado ao pino 9
    mServo.write(100); // Cancela fechada
    pinMode (E, INPUT);
    pinMode (Ir, INPUT);

    lcd.setCursor(0,0);
    lcd.print("     FFILIPE    ");
    lcd.setCursor(0,1);
    lcd.print(" PARQUE ");
    delay(2000);
    lcd.clear();  
}

void loop() {
    
    int Estado1 = digitalRead(Ir);
    
    if (Estado1==LOW && flag1 == 0) 
    {                                       
        // Se detetar um carro a menos de 10 cm e a tentar entrar no Parque
        if (Lugares_Estacionamento >= 1) 
        {
            flag1 = 1;
            if (flag2 == 0) {
                for(pos = 100; pos >= 0; pos--)
                {
                    mServo.write(pos);
                    delay(2);
                }
                Lugares_Estacionamento = Lugares_Estacionamento - 1;
            }
        } 
        else 
        { 
            lcd.setCursor(0,0);
            lcd.print("    Desculpe    ");  
            lcd.setCursor(0,1);
            lcd.print("Parque Completo! "); 
            digitalWrite(Led_Vermelho, HIGH);
            digitalWrite(Led_Verde, LOW);
            delay(3000);
            lcd.clear(); 
        }
    }
    int Estado = digitalRead(E);
    if (Estado==LOW && flag2 == 0) 
    { 
        // Se detetar um carro a sair do Parque
        flag2 = 1;
        if (Lugares_Estacionamento <=3)
        {
           if (flag1 == 0) 
           {   
              for(pos = 100; pos >= 0; pos--)
              {
                  mServo.write(pos);
                  delay(2);
              }              
              Lugares_Estacionamento = Lugares_Estacionamento + 1;
           }
        }
    }

    if (flag1 == 1 && flag2 == 1) 
    {
        for(pos = 0; pos < 100; pos++)
        {
            mServo.write(pos);
            delay(2);
        }
        flag1 = 0;
        flag2 = 0;
    }
    lcd.setCursor(0,0);
    lcd.print("    ESBN PARK    ");
    lcd.setCursor(0,1);
    lcd.print("Lugares : ");
    lcd.print(Lugares_Estacionamento);

    // Atualiza o Estado dos Leds indicando se existem Lugares ou não no Parque
    if (Lugares_Estacionamento > 0) 
    {
        digitalWrite(Led_Verde, HIGH);
        digitalWrite(Led_Vermelho, LOW);
    } 
    else 
    {
        digitalWrite(Led_Verde, LOW);
        digitalWrite(Led_Vermelho, HIGH);
    }
    delay(1200);
}
