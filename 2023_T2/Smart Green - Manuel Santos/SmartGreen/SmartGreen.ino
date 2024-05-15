#include <WiFi.h>
#include <HTTPClient.h>
WiFiClient client;
#include <PubSubClient.h>
const char* BROKER_MQTT = "broker.mqtt-dashboard.com";
int BROKER_PORT = 1883;

#define ID_MQTT  "SSL_SmartGreen_ESP32"
#define TOPIC_PUBLISH "SSL_SmartGreen"
PubSubClient MQTT(client);

//Bluetooth
#include "BluetoothSerial.h"
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;

// intervalos de leitura:
unsigned long anteriorMillis = 0;
unsigned long intervaloMillis = 3000;
unsigned long atualMillis = 0;

//rega
byte rega = 0, regaAUX = 0;
byte limiteS = 0;
byte limiteI = 0;
byte limiteRega = 30;
const int recebeI = 2;
const int enviaI = 15;
const int recebeS = 16;
const int enviaS = 4;
boolean autoRega = false;
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "";
const char* password = "";
int port = 80;

WebServer server(port);

//const int led = 2;

//DHT11 things
#include "DHT.h"
#define DHTPIN 22
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//Other things
float asoilmoist = analogRead(32);
float asoilmoist2 = analogRead(32);
float asloi_aux;
float temp;
float hum;
void handleRoot() {
  //digitalWrite(led, 1);
  String webtext ;
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  if (isnan(hum) || isnan(temp)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  webtext = "<html>\
  <head>\
    <meta http-equiv='refresh' content='5'/>\
    <title>WEMOS HIGROW ESP32 WIFI SOIL MOISTURE SENSOR</title>\
    <style>\
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
    </style>\
  </head>\
  <body>\
    <h1>SMART GREEN .:AEGN:.</h1>\
    <br>\
    <p>Estufa automatizada do Agrupamento de Escolas de Gafanha da Nazare, desenvolvida no ambito do projeto Smart Gadgets & Robotica, com o apoio do projeto Smart School Lab</p>\
    <p>Humidade do solo: valores elevados (intervalo de +/-3344) significa que o solo esta seco, valores baixos (+/- 2000)significa que o solo esta humido.</p>\
    <br>\
    <p>Data/Hora: <span id='datetime'></span></p><script>var dt = new Date();document.getElementById('datetime').innerHTML = (('0'+dt.getDate()).slice(-2)) +'.'+ (('0'+(dt.getMonth()+1)).slice(-2)) +'.'+ (dt.getFullYear()) +' '+ (('0'+dt.getHours()).slice(-2)) +':'+ (('0'+dt.getMinutes()).slice(-2));</script>\
    <br>\
    <p>Humidade do Solo: " + String((int) asoilmoist) + " %</p>\
    <p>Humidade do Solo: " + String((int) asloi_aux) + " %</p>\
    <p>Temperatura do ar: "   + String(temp) + " &#176;C</p>\
    <p>Humidade do ar: "   + String(hum) + " %</p>\
  </body>\
</html>";
  server.send(200, "text/html", webtext);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void)
{
  pinMode(enviaI, OUTPUT);
  pinMode(recebeI, INPUT);
  pinMode(recebeS, INPUT);
  pinMode(enviaS, OUTPUT);
  digitalWrite(enviaI, 1);
  digitalWrite(enviaS, 1);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  server.on("/", handleRoot);
  server.on("/inline", []() {
    server.send(200, "text/plain", "this works as well");
  });
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
  dht.begin();
  delay(2000);
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  SerialBT.begin("SmartGreenSensor 01");
}

void loop(void)
{
  if (autoRega == false)
  {
    if (digitalRead(recebeI))
    {
      limiteI = 1;
    }
    else
    {
      limiteI = 0;
    }
    if (digitalRead(recebeS))
    {
      limiteS = 1;
    }
    else
    {
      limiteS = 0;
    }
  }
  uint16_t soil = analogRead(32);
  float soil_aux;
  asoilmoist = map(soil, 0, 4095, 100, 0);
  asoilmoist2 = 0.95 * asoilmoist2 + 0.05 * analogRead(32);
  soil_aux = asoilmoist2;
  asloi_aux = map(soil_aux, 0, 4095, 100, 0);
  Serial.println("Valor 1\tValor 2\tLeitura");
  Serial.print(asoilmoist); Serial.print(" %"); Serial.print("\t");
  Serial.print(asoilmoist2); Serial.print(" %"); Serial.print("\t");
  Serial.println(soil);
  server.handleClient();
  atualMillis = millis();
  if (atualMillis  - anteriorMillis >= intervaloMillis)
  {
    handleRoot();
    anteriorMillis = atualMillis;
    mantemConexoes();
    MQTT.loop();
    enviaValores_simples();
  }
  if (SerialBT.available())
  {
    char recebeBT = SerialBT.read();
    SerialBT.println(recebeBT);
    int recebeBT2 = SerialBT.parseInt();
    SerialBT.println(recebeBT2);
    if((recebeBT == 't') || (recebeBT == 'T'))
    {
      autoRega = recebeBT2;
    }
    else if ((recebeBT == 'a') || (recebeBT == 'A'))
    {
      intervaloMillis = recebeBT2 * 1000;
    }
    else if ((recebeBT == 'r') || (recebeBT == 'R'))
    {
      regaAUX = recebeBT2;
    }
    else if (recebeBT == 'l')
    {
      limiteI = recebeBT2;
    }
    else if (recebeBT == 'L')
    {
      limiteS = recebeBT2;
    }
    else if ((recebeBT == 'h') || (recebeBT == 'H'))
    {
      limiteRega = recebeBT2;
    }
    else
    {
      SerialBT.println("Comando inválido. 'a' ou 'A' para alterar a frequência de atualização; 'r' ou 'R' para alterar rega; 'l' ou 'L' para alterar sensores de nível de água; 'h' ou 'H' masi valor de limite para ativar a rega.");
    }
    SerialBT.flush();
  }
}
