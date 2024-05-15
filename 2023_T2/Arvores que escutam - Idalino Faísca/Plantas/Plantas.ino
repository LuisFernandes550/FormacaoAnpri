
/* -----------------------------------------------------------------------------------------------------------
 * 
 * “ÁRVORES que ESCUTAM”
 * Projeto Cultural de Escola - Agrupamento de Escolas Fernão do Pó
 * Medição da produção de oxigénio de uma planta em função do som ambiente (ex: música e voz humana).
 * Medição dos parâmetros ambientais: Temperatura (ºC); Humidade relativa (%); Luminisidade (lux).
 * 
 * Idalino Faísca
 * idalinofaisca@aefp.pt
 * 
 * --------------------------------------------------------------------------------------------------------- */


/* Gravity I2C Oxygen Sensor */
/*!
 * @file getOxygenData.ino
 * @brief Read oxygen concentration ,The unit is concentration percent (% vol).
 * @n step: we must first determine the iic device address, will dial the code switch A0, A1 (ADDRESS_0 for [0 0]),
 * (ADDRESS_1 for [1 0]), (ADDRESS_2 for [0 1]), (ADDRESS_3 for [1 1]).
 * @n Then calibrate the oxygen sensor
 * @n note: it takes time to stable oxygen concentration, about 10 minutes.
 * @n The experimental phenomenon is that a certain percentage of oxygen concentration is printed on the serial port.
 * @n Because the oxygen concentration in oxygen air is certain, the data will not be greater than 25% vol.
 * @copyright Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @license The MIT License (MIT)
 * @author ZhixinLiu(zhixin.liu@dfrobot.com)
 * @version V1.0
 * @date 2021-10-22
 * @url https://github.com/DFRobot/DFRobot_OxygenSensor
 */

// I2C Slave Address, The default is ADDRESS_3.
// ADDRESS_0   0x70  i2c device address.
// ADDRESS_1   0x71
// ADDRESS_2   0x72
// ADDRESS_3   0x73

#include "DFRobot_OxygenSensor.h"
#define Oxygen_IICAddress ADDRESS_3
#define COLLECT_NUMBER 10  // collect number, the collection range is 1-100.
DFRobot_OxygenSensor oxygen;

/* Temperature humidity sensor HHT22 */
#include <DHT.h>
#define datapin 2      // Sensor DHT22 ligado ao pino D2
#define DHTTYPE DHT22  // Sensor de Temperatura e humidade definido como DHT22
DHT dht(datapin, DHTTYPE);
float temp;
float humid;

/* Light sensor Adafruit BH1750 */
//for help look at: https://github.com/Starmbi/hp_BH1750/wiki
//The I2C address address for the BH1750 is 0x23 and can be switched to 0x5C by pulling the address pin high to VCC
#include <Arduino.h>
#include <hp_BH1750.h>  //  include the library
hp_BH1750 BH1750;       //  create the sensor

/* LCD Screen I2C 20X4 */
#include <Wire.h>               // Biblioteca para comunicação I2C
#include <LiquidCrystal_I2C.h>  // Biblioteca para LCD I2C
//LiquidCrystal_I2C lcd(0x27, 16, 2);                 // LCD 16x2 no endereço 0x27
LiquidCrystal_I2C lcd(0x27, 20, 4);  // LCD 20x4 no endereço 0x27
//LiquidCrystal_I2C lcd(0x3F,16,2);

// make some custom characters: icon para %O2; Temperatura, Humidade e Luminosidade; letras acentuadas
byte cOxig[8] = {  // Oxigenio
  B00100,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110,
  B00100,
  B01000
};

byte cTemp[8] = {  // Temperatura
  B00000,
  B00100,
  B01010,
  B01010,
  B01110,
  B01110,
  B11111,
  B01110
};

byte cHumi[8] = {  // Humidade
  B00000,
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B01110
};

byte cLumi[8] = {  // Luminosidade
  B00000,
  B00100,
  B10001,
  B01110,
  B11011,
  B01110,
  B10001,
  B00100
};


byte cEac[8] = {  // Letra "é" com acento
  B00010,
  B00100,
  B01110,
  B10001,
  B11111,
  B10000,
  B01110,
  B00000
};


byte cEle[8] = {  // Letra "l" minúsculo
  B00100,
  B01010,
  B10010,
  B10010,
  B10100,
  B10000,
  B01100,
  B00000
};

byte cAace[8] = {  // Letra "A" com acento
  B00010,
  B01110,
  B10001,
  B10001,
  B11111,
  B10001,
  B10001,
  B00000
};

byte cOace[] = {  // Letra "O" com acento
  B00001,
  B01110,
  B10001,
  B10001,
  B10001,
  B10001,
  B01110,
  B00000
};

void setup() {

  lcd.init();

  lcd.createChar(0, cOxig);
  lcd.createChar(1, cTemp);
  lcd.createChar(2, cHumi);
  lcd.createChar(3, cLumi);
  lcd.createChar(4, cEac);
  lcd.createChar(5, cEle);
  lcd.createChar(6, cAace);
  lcd.createChar(7, cOace);

  Serial.begin(9600);  // Inicia comunicação Série a 9600 baud

  while (!oxygen.begin(Oxygen_IICAddress)) {
    //Serial.println("Oxygen Sensor: I2C device number error !");
    delay(1000);
  }
  //Serial.println("Oxygen Sensor: I2C connect success !");

  dht.begin();  // Inicia sensor DHT22

  bool avail = BH1750.begin(BH1750_TO_GROUND);  // init the sensor with address pin connetcted to ground
                                                // result (bool) wil be be "false" if no sensor found

  //Serial.println("Temperatura: Humidade:");       // print some text in Serial Monitor
  //lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("*** Agrupamento ****");
  lcd.setCursor(0, 1);
  lcd.print("*    de Escolas    *");
  lcd.setCursor(0, 2);
  lcd.print("*   FERNAO do P");
  lcd.write(byte(7));
  lcd.print("   *");
  lcd.setCursor(0, 3);
  lcd.print("***** Bombarral ****");
  //lcd.setCursor(0,1);
  delay(10000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("====================");
  lcd.setCursor(0, 1);
  lcd.print("  Projeto Cultural  ");
  lcd.setCursor(0, 2);
  lcd.write(byte(6));
  lcd.print("RVORES que ESCUTAM!");
  lcd.setCursor(0, 3);
  lcd.print("====================");
  //lcd.setCursor(0,1);
  delay(10000);
}

void loop() {

  float oxygenData = oxygen.getOxygenData(COLLECT_NUMBER);

  /* Find Temperature & Humidity */
  temp = dht.readTemperature();
  humid = dht.readHumidity();

  BH1750.start();               // starts a measurement
  float lux = BH1750.getLux();  //  waits until a conversion finished


  /* Print Output on LCD Screen */
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(byte(0));
  lcd.setCursor(1, 0);
  //lcd.print(String(" Oxigenio: ") + String(oxygenData,1) + ("%"));
  lcd.print(" Oxig");
  lcd.setCursor(6, 0);
  lcd.write(byte(4));
  lcd.print(String("nio : ") + String(oxygenData, 1) + ("%"));  // % oxigénio com 1 casa decimal
  lcd.setCursor(0, 1);
  lcd.write(byte(1));
  lcd.setCursor(1, 1);
  lcd.print(String(" Temp.    : ") + String(temp, 1) + ((char)223) + ("C"));  // char(223) é o código ASCII de "grau", º
  lcd.setCursor(0, 2);
  lcd.write(byte(2));
  lcd.setCursor(1, 2);
  lcd.print(String(" Humidade : ") + String(humid, 1) + ("%"));  // Humidade relativa com 1 casa decimal
  lcd.setCursor(0, 3);
  lcd.write(byte(3));
  lcd.setCursor(1, 3);
  //lcd.print(String(" Luz: ") + String(lux,0) + (" lx"));
  lcd.print(" Luz      : ");
  lcd.print(lux, 0);
  lcd.write(byte(5));
  lcd.print("x");

  /* Print Output on Serial Monitor */
  Serial.print("Oxigénio(%): ");
  Serial.println(oxygenData);
  //Serial.println(" %vol");
  //Serial.print(",");
  //Serial.print("Temperatura(ºC): "); //display the temperature
  //Serial.print(temp, 1);
  //Serial.print(",");
  //Serial.print("Humidade(%): "); //output value humidity
  //Serial.print(humid, 1);
  //Serial.print(",");
  //Serial.println(lux);

  delay(2000);
}
