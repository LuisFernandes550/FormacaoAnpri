
#include "DHT.h"  
#include<Wire.h>
#include <EasyTransfer.h> //para comunicação entre os HC-05

#define DHTPIN 12 
#define DHTTYPE DHT11 

float temp, hum;

struct ENVIO_DADOS
{
  float temperatura ;
  float humidade ;
};

struct RECEBIDO
{
  boolean recebido = false;
};

int contador = 0;
ENVIO_DADOS data;
RECEBIDO recebido;
DHT dht(DHTPIN, DHTTYPE);
EasyTransfer ETin, ETout; //são necessários dois objectos EasyTransfer,
//um para os dados enviados e outro para os dados recebidos.

void setup() {
  Serial.begin(9600); //Baudrate dos módulos Bluetooth 
  dht.begin();
  ETout.begin(details(data), &Serial);
  ETin.begin(details(recebido), &Serial);
}

void loop() {
  if(ETin.receiveData())
  {
    if(recebido.recebido == true)
    {
       digitalWrite(13,HIGH);
       delay(100);
       digitalWrite(13,LOW);
    }
  }
  contador ++;   
  Serial.println(contador); 
  if(contador == 8)//Send data once every two seconds
  {
   temp = dht.readTemperature();
   hum = dht.readHumidity();
   Serial.println(temp); 
   Serial.println(hum); 
       
   data.temperatura = dht.readTemperature();
   data.humidade = dht.readHumidity();
   ETout.sendData();
   contador = 0;
  }
  delay(250);
  recebido.recebido = false;
}
