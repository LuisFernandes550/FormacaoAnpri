//Parecido com o led correndo mas com mais leds e cada led vai diminuindo um pouco a intensidade

//https://www.youtube.com/BitaBit

//Para mais informações sobre este código acesse:
//https://youtu.be/U7RcNgb9qBE

//https://bitabittecnologia.com.br

#include <FastLED.h>

//Quantidade de LEDs na fita
#define NUM_LEDS 100

//CRGBArray de LEDs
CRGBArray<NUM_LEDS> leds; //outro tipo de array diferente de CRGB leds[NUM_LEDS];

void setup()
{ 
  //FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, 5, GRB>(leds, NUM_LEDS); //DATA_PIN
}
void loop()
{ 
  for(int i=0; i<NUM_LEDS; i++) 
  {   
    leds.fadeToBlackBy(10); //Quanto maior o nº menos leds ficam acesos e qt menor mais leds ficam acesos

    leds[i] = CRGB::Green;
    FastLED.show();
    delay(50);
  }
}

