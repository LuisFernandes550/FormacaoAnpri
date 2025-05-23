#include <FastLED.h>

//https://www.youtube.com/BitaBit

//Para mais informações sobre este código acesse:
//https://youtu.be/U7RcNgb9qBE

//https://bitabittecnologia.com.br

#include <FastLED.h>

//Quantidade de LEDs na fita
#define NUM_LEDS 100

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
  for(int i=0; i<=NUM_LEDS; i++)
  {
    leds[i] = CRGB::Green;

    if(i != 0) 
      leds[i-1] = CRGB::Black;

    FastLED.show();
  }
}
