#define RELE 25
bool flagRele = false;
bool flagBT = false;
bool flagBTN = false;
int botaoSolenoide = 14;
int litrosMaximo = 300;
byte sensorInterrupt = 12;
byte sensorPin       = 12;
float calibrationFactor = 4.5;

volatile byte pulseCount;

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

unsigned long oldTime;

float contaLitros = 0.0;
// intervalos de leitura:
unsigned long anteriorMillis = 0;
const long intervaloMillis = 3000;
unsigned long atualMillis = 0;


#include <WiFi.h>
#include <HTTPClient.h>
WiFiClient client;
const char* ssid     = "";
const char* password = "";


#include <PubSubClient.h>
const char* BROKER_MQTT = "broker.mqtt-dashboard.com";
int BROKER_PORT = 1883;
#define ID_MQTT  "SSL_SmartWaterSense_01_ESP32"
#define TOPIC_PUBLISH "SSL_SmartWaterSense"
PubSubClient MQTT(client);

//Bluetooth
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

void setup()
{
  pinMode(botaoSolenoide, INPUT);
  pinMode(RELE, OUTPUT);
  digitalWrite(RELE, LOW);
  flagRele = true;
  flagBTN = digitalRead(botaoSolenoide);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);

  SerialBT.begin("SmartWaterSensor 01");
}

/**
   Main program loop
*/
void loop()
{
  //verificar se já atingiu o limite de 300 litros
  if (litrosMaximo < (int)contaLitros)
  {
    flagRele = false;
  }
  else
  {
    flagRele = true;
  }
  if (digitalRead(botaoSolenoide))
  {
    flagBTN = true;
  }
  else
  {
    flagBTN = false;
  }
  if (SerialBT.available())
  {
    char recebeBT = SerialBT.read();
    switch (recebeBT)
    {
      case '1': //ligar a solenoide
        flagBT = true;
        break;
      case '0': //desligar a solenoide
        flagBT = false;
        break;
      case 'a'://alterar o limite de litros
        {
          int recebeBT2 = SerialBT.parseInt();
          litrosMaximo = recebeBT2;
        }
    }
  }

  atualMillis = millis();
  if (atualMillis  - anteriorMillis >= intervaloMillis)
  {
    anteriorMillis = atualMillis;
    mantemConexoes();
    MQTT.loop();
  }

  if ((flagRele) || (flagBT) || (flagBTN))
  {
    if ((millis() - oldTime) > 1000)
    {
      detachInterrupt(sensorInterrupt);
      flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
      oldTime = millis();
      flowMilliLitres = (flowRate / 60) * 1000;
      totalMilliLitres += flowMilliLitres;
      unsigned int frac;
      Serial.print("Flow rate: ");
      Serial.print(int(flowRate)); 
      Serial.print("L/min");
      Serial.print("\t");  
      Serial.print("Output Liquid Quantity: ");
      Serial.print(totalMilliLitres);
      Serial.println("mL");
      Serial.print("\t");       // Print tab space
      Serial.print(totalMilliLitres / 1000);
      Serial.println("L");
      contaLitros = (float) totalMilliLitres / 1000;
      enviaValores_simples(contaLitros, flowRate);
      pulseCount = 0;
     attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
    }
  }
  else
  {
    enviaMensagem("Atingiu o limite de 300 litros | fluxo: 0");
  }
}

void pulseCounter()
{
  pulseCount++;
}
