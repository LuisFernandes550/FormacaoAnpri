// Bibliotecas necessárias
#include "DFRobot_PH.h"
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Cabo de dados está conectado à porta 10 do Arduino
#define DS18B20 10

// Configuração da instância oneWire para se comunicar com qualquer dispositivo OneWire (não apenas com ICs de temperatura Maxim/Dallas)
OneWire ourWire(DS18B20);

// Referência oneWire para temperatura de Dallas.
DallasTemperature sensors(&ourWire);

#define PH_PIN A1
float voltage, phValue, temperature = 25; // Corrigido para definir a variável "temperature" no escopo global
DFRobot_PH ph;
SoftwareSerial bluetooth(2, 3);  // RX, TX

void setup() {
  Serial.begin(115200);     // Inicia a comunicação serial com o PC
  bluetooth.begin(115200);  // Inicia a comunicação serial com o módulo Bluetooth
  ph.begin();
  sensors.begin();  // Inicia o sensor de temperatura
  delay(1000);
}

void loop() {
  sensors.requestTemperatures();
  static unsigned long timepoint = millis();
  if (millis() - timepoint > 1000U) {  //intervalo de tempo: 1s
    timepoint = millis();
   
    temperature = sensors.getTempCByIndex(0); // Corrigido para atualizar a temperatura a cada ciclo
    voltage = analogRead(PH_PIN) / 1024.0 * 5000;  // Lê a tensão
    phValue = ph.readPH(voltage, temperature);     // Converte a tensão em pH com compensação de temperatura
    Serial.print("Temperatura: ");
    Serial.print(temperature, 1);
    Serial.print("ºC  pH:");
    Serial.println(phValue, 2);
  }
  ph.calibration(voltage, temperature);  // Processo de calibração

  // Verifica se existem dados disponíveis por Bluetooth
  if (bluetooth.available()) {
    char c = bluetooth.read();
    if (c == 'c') {  // Se for recebido 'c', inicia a calibração
      Serial.println("Inicializando a calibração...");
      ph.calibration(voltage, temperature);
    }
  }
}
