//Mostra como se altera o brilho/intensidade do led

//https://www.youtube.com/BitaBit

//Para mais informações sobre este código acesse:
//https://youtu.be/U7RcNgb9qBE

//https://bitabittecnologia.com.br

#include <FastLED.h>

//Quantidade de LEDs na fita
#define NUM_LEDS 100

#define DATA_PIN 5

//Array de LEDs
CRGB leds[NUM_LEDS];

void setup() 
{ 
  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);

  //Define a cor para todos os LEDs
  for( int i=0; i<NUM_LEDS; i++ )
  {
    leds[i] = CRGB::Blue;
  }

  //Seta o brilho para todos os LEDs com intensidade 0
  FastLED.setBrightness(0);

  FastLED.show();
}

void loop() 
{ 
  //Aumenta a intensidade gradativamente
  for(int i=0; i<=255; i++)
  {
    FastLED.setBrightness(i); //Valores de 0 a 255
    FastLED.show();
    delay(10);
  }

  //Diminui a intensidade gradativamente
  for(int i=255; i>=0; i--)
  {
    FastLED.setBrightness(i); //Valores de 0 a 255
    FastLED.show();
    delay(10);
  }

    delay(500);
}