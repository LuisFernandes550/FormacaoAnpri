//TE AQUARIO 


#include "thingProperties.h"

#include <LiquidCrystal_I2C.h>                    

#define TdsSensorPin A2                       
#define VREF 5.0                              
#define SCOUNT  30                            
int analogBuffer[SCOUNT];                     
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0,copyIndex = 0;
float averageVoltage = 0,tdsValue = 0,temperature = 25;                                              

# define pinLuzReleLed 5                      
                                             
int water_ain=A3;                             
float ad_value=0;
                                                                                      
#include "DFRobot_PH.h"                             
#include <EEPROM.h>

#define PH_PIN A1                             
float voltage,phValue,temperaturePH = 25;    

DFRobot_PH ph;                                                                          
                                                                                        
#include <OneWire.h>                          
#include <DallasTemperature.h>                
#define sensorDS18B20 2                       
OneWire ourWire(sensorDS18B20);              
DallasTemperature sensors(&ourWire);         

float tempAgua;                               
int tempAguaInt=tempAgua;                                                  
String estadoTermostato="OFF";                
# define pinReleTermostato 4                  
# define pinReleLed 5                                                           

String alarmeTA;                                
String alarmeTDS;                               
String alarmePH;                                
String alarmeNA;                                
String estadoAlarmeNenhum;                      

# define pinInpBotao1 8                         
boolean EstAtuBotao1=true; 

# define pinBuzzer 3                        
int timerBuzzer=0;                        

LiquidCrystal_I2C lcdI2C1(0x26, 16, 3);       
LiquidCrystal_I2C lcdI2C2(0x27, 16, 3);       


void setup() {

    pinMode(TdsSensorPin,INPUT);                            
    pinMode(pinReleTermostato, OUTPUT);
    pinMode(pinReleLed, OUTPUT);
    pinMode(pinInpBotao1,INPUT_PULLUP);
    pinMode(pinBuzzer, OUTPUT);                      
    digitalWrite(pinReleTermostato,LOW);
    digitalWrite(pinReleLed, HIGH);

                                                          
                                                            
lcdI2C1.backlight();                                       
lcdI2C2.backlight();                                        
lcdI2C1.clear();                                            
lcdI2C2.clear();                                                          
lcdI2C1.init();                                                    
lcdI2C2.init();                                             

Serial.begin(9600);                                         
sensors.begin();                                   
ph.begin();                                           


  initProperties();
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
  
  
  
}

void loop() {
  
  ArduinoCloud.update();
  
  leituraSensorNivelLiquidos();
  leituraSensorPH();                                  
  leituraSensorDS18B20();                                                           
  leituraSensorTDS();                                  
  escreveDados();                                     
  onTempAguaMedidorIotChange();
  onNivelAguaMedidorIotChange();
  onAlarmesTextoIotChange();

  delay(1000);

  if ( alarmeTA!="T AGUA" && alarmeNA!="N AGUA" && alarmeTDS!="TDS" && alarmePH!="PH" )  {

    lcdI2C1.setCursor(0,3); 
    lcdI2C1.print("NENHUM");
    
    digitalWrite(pinBuzzer,LOW);
    timerBuzzer=0;
        
      }else{
           lcdI2C1.setCursor(0,3);
           lcdI2C1.print("       ");
          onAlarmesTextoIotChange();
         
           timerBuzzer++;
   
                if (timerBuzzer==1) {
                digitalWrite(pinBuzzer,HIGH); 
                }

                if (timerBuzzer==2) {
                digitalWrite(pinBuzzer,LOW);
                onAlarmesTextoIotChange();
                timerBuzzer=0; 
           }
      }


 EstAtuBotao1 = digitalRead(pinInpBotao1);                      
  
                                                                   
     if  (EstAtuBotao1 == false)   {                              
          if(digitalRead(pinReleLed)==true)  {
          digitalWrite(pinReleLed, LOW);
          }else{
                      digitalWrite(pinReleLed, HIGH);
          }
         delay (250);                                                                                    
      }
                    



void leituraSensorTDS(){

  static unsigned long analogSampleTimepoint = millis();
   if(millis()-analogSampleTimepoint > 40U)    
   {
     analogSampleTimepoint = millis();
     analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);   
     analogBufferIndex++;
     if(analogBufferIndex == SCOUNT) 
         analogBufferIndex = 0;
   }   
   static unsigned long printTimepoint = millis();
   if(millis()-printTimepoint > 800U)
   {
      printTimepoint = millis();
      for(copyIndex=0;copyIndex<SCOUNT;copyIndex++)
        analogBufferTemp[copyIndex]= analogBuffer[copyIndex];
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0;
      float compensationCoefficient=1.0+0.02*(temperature-25.0);   
      float compensationVolatge=averageVoltage/compensationCoefficient;  
      tdsValue=(133.42*compensationVolatge*compensationVolatge*compensationVolatge - 255.86*compensationVolatge*compensationVolatge + 857.39*compensationVolatge)*0.5; 
      Serial.print(tdsValue,0);
      Serial.println("ppm");

        if (tdsValue >  1500 )  {
        alarmeTDS="TDS";     
            }else  {
                    alarmeTDS="   "; 
                   }           
       }
  }
   

int getMedianNum(int bArray[], int iFilterLen) 
{
      int bTab[iFilterLen];
      for (byte i = 0; i<iFilterLen; i++)
      bTab[i] = bArray[i];
      int i, j, bTemp;
      for (j = 0; j < iFilterLen - 1; j++) 
      {
      for (i = 0; i < iFilterLen - j - 1; i++) 
          {
        if (bTab[i] > bTab[i + 1]) 
            {
        bTemp = bTab[i];
            bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
         }
      }
      }
      if ((iFilterLen & 1) > 0)
    bTemp = bTab[(iFilterLen - 1) / 2];
      else
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
      return bTemp;
}


void leituraSensorDS18B20() {                        
  
  sensors.requestTemperatures();                      
  tempAgua = sensors.getTempCByIndex(0);              
  tempAguaInt=tempAgua;

    if (tempAguaInt < 18 && estadoTermostato == "OFF") {    
      digitalWrite(pinReleTermostato,HIGH);
      estadoTermostato="ON";   
        }

     if (tempAguaInt >= 20 &&  estadoTermostato == "ON") {  
      digitalWrite(pinReleTermostato,LOW);
      estadoTermostato="OFF";   
        }


     if (tempAguaInt < 15 || tempAguaInt >= 23)  {
        alarmeTA="T AGUA";     
     } else  {
                alarmeTA="      "; 
            }
    }


void leituraSensorNivelLiquidos() { 
      ad_value=analogRead(water_ain);
      if(ad_value>200)
      {
      Serial.println("High water level");
      } else {
    Serial.println("Low water level"); }
      delay(500);
      ad_value = ad_value*5/1024;
      Serial.println(ad_value);

              if (ad_value <  2 )  {
        alarmeNA="N AGUA";     
            }else  {
                    alarmeNA="      "; 
                   }

}


void escreveDados() {                                 
                                                      
    Serial.print("Temperatura aquário: ");                    
    Serial.println(tempAgua);                                 
    Serial.println("ºC") ;  
                                                      
    lcdI2C1.setCursor(0,0);                           
    lcdI2C1.print("AQUARIUM CONTROL 3TE");              
    lcdI2C1.setCursor(0,1);               
    lcdI2C1.print("                    "); 
    
    lcdI2C1.setCursor(0,2);                           
    lcdI2C1.print("Alarmes:"); 
    lcdI2C1.setCursor(9,2); 
    lcdI2C1.print(alarmeTA); 
    lcdI2C1.setCursor(17,2); 
    lcdI2C1.print(alarmeTDS);  
    lcdI2C1.setCursor(9,3); 
    lcdI2C1.print(alarmeNA); 
    lcdI2C1.setCursor(18,3); 
    lcdI2C1.print(alarmePH);                  
 
    lcdI2C2.setCursor(0,0);               
    lcdI2C2.print("T AGUA= ");   
    lcdI2C2.print(tempAgua); 
    lcdI2C2.print(" C"); 
    lcdI2C2.setCursor(17,0);
    lcdI2C2.print("   ");
    if (estadoTermostato=="ON") {
        lcdI2C2.setCursor(18,0);
        lcdI2C2.print(estadoTermostato);
        } else {
                lcdI2C2.setCursor(17,0);  
                lcdI2C2.print(estadoTermostato);
                }

    lcdI2C2.setCursor(0,1);                                     
    lcdI2C2.print("N AGUA= ");                             
    lcdI2C2.print(ad_value) ;

    lcdI2C2.setCursor(0,2);                                     
    lcdI2C2.print("TDS= ");                          
    lcdI2C2.print(tdsValue) ;
    lcdI2C2.print(" ppm"); 

    lcdI2C2.setCursor(0,3);                                     
    lcdI2C2.print("PH= ");                             
    //lcdI2C2.print(phValue,2) ;
    lcdI2C2.print("6.7");          
  
}

void onTempAguaMedidorIotChange()  {

  tempAgua_medidor_iot=tempAgua;
  
}


void onLedInterruptorIolChange()  {

 if(digitalRead(pinReleLed)==true)  {

          digitalWrite(pinReleLed, LOW);
          }else{
                      digitalWrite(pinReleLed, HIGH);
          }
               
         delay (250);   
                                                              
  
}


void onNivelAguaMedidorIotChange()  {
nivelAgua_medidor_iot=ad_value;


}


void onAlarmesTextoIotChange()  {
  
  alarmes_texto_iot=("ALARMES: "+alarmeTA+" "+alarmeNA+" "+alarmeTDS+" "+alarmePH);
  

}
