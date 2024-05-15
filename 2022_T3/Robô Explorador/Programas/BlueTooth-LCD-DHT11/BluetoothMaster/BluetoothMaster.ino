
#include <EasyTransfer.h> //https://github.com/madsci1016/Arduino-EasyTransfer
#include<Wire.h>
#include<LiquidCrystal_I2C.h>

//inicializa o display
LiquidCrystal_I2C lcd(0x27, 16, 2);

char temperaturaChar[10];
char humidadeChar[10];

struct DADOS_AMBIENTE
{
  float temperatura ;
  float humidade ;
};

struct RECEBIDO
{
  boolean recebido = false;
};

DADOS_AMBIENTE data;
RECEBIDO recebido;

EasyTransfer ETin, ETout; 


void setup() {

  Serial.begin(9600);
  Serial.begin(115200);
  //*************** Inicia LCD ***********
  lcd.init(); 
  lcd.backlight();// acende a luz de fundo
  
  ETin.begin(details(data), &Serial);
  ETout.begin(details(recebido), &Serial);
}

void loop() {
    lcd.clear(); //limpa o LCD   
    lcd.setCursor(0,0);
    lcd.print("Aguardando dados"); 
    

  if(ETin.receiveData()){
    lcd.clear(); //limpa o LCD  

    String humidadeString = String(data.humidade,1);
    humidadeString.toCharArray(humidadeChar,10);

    lcd.setCursor(0,0);
    lcd.print("Humidade: "); 
    lcd.print(humidadeChar);  
    lcd.print("%");

    Serial.print("Humidade: ");
    Serial.println(humidadeChar);
     
    String temperaturaString = String(data.temperatura,1);
    temperaturaString.toCharArray(temperaturaChar,10);    
 
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(temperaturaChar);  
    //lcd.print((char) 223);
    lcd.print("C");
    
    recebido.recebido = true;
    ETout.sendData();
    } 
    delay(200);
    recebido.recebido = false;
}
