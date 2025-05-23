//https://www.youtube.com/BitaBit

//Para mais informações sobre este código acesse:
//https://youtu.be/U7RcNgb9qBE

//https://bitabittecnologia.com.br

#include <FastLED.h>

//Quantidade de LEDs na fita
#define NUM_LEDS 1

#define DATA_PIN 3

//Array de LEDs
CRGB leds[NUM_LEDS];

void setup() 
{ 
  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
}

void loop() 
{ 
  //Definir cor usando Códigos de cores HTML
  leds[0] = CRGB::Purple; //CTRL + click na cor abre um fich crgb.h com muitas cores
  FastLED.show();
  delay(500);
  
  leds[0] = CRGB::Black;
  FastLED.show();
  delay(500);
    
  //Definir cor usando Hexadecimal
  //leds[0] = 0X0000ff;
  //FastLED.show();
  //delay(500);
    
  //leds[0] = 0Xffff00;
  //FastLED.show();
  //delay(500);

  //Definir cor usando RGB
  //leds[0] = CRGB(0, 0, 255);
  //FastLED.show();
  //delay(500);
    
  //leds[0] = CRGB(255, 0, 102);
  //FastLED.show();
  //delay(500);
}