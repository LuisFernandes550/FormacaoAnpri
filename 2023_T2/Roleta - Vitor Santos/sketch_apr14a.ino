#include "FastLED.h"
#define NUM_LEDS 60
#define DATA_PIN A0
#define SCORE_PIN 6
#define SCORE_LEDS 6
#define BRIGHTNESS 60
CRGB leds[NUM_LEDS];
CRGB sleds[NUM_LEDS];

bool reachedEnd = false;
byte gameState = 0;
int period = 1000;
unsigned long time_now = 0;
byte Position = 0;
byte level = 0;

const byte ledSpeed[6] = {50, 40, 30, 20, 14, 7};

bool findRandom = false;
byte spot = 0;

void setup() {
  // put your setup code here, to run once:
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.addLeds<WS2812B, SCORE_PIN, GRB>(sleds, SCORE_LEDS);
  pinMode(A3, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.println("Reset");
}

void loop() {
  // put your main code here, to run repeatedly:
  FastLED.setBrightness(BRIGHTNESS );
  if (gameState == 0) {
    fill_rainbow(leds, NUM_LEDS, 0, 20); //2 = longer gradient strip
    fill_rainbow(sleds, SCORE_LEDS, 0, 40); //2 = longer gradient strip

    if (digitalRead(A3) == LOW) {
      Position = 0;
      findRandom = true;
      delay(500);
      for (byte i = 0; i < NUM_LEDS; i++) {
        leds[i].setRGB(0, 0, 0);
        delay(40);
        FastLED.show();
       int thisPitch = map (i, 60, 0, 100, 1500);
       tone(9, thisPitch,120);
      }
      for (byte i = 0; i < SCORE_LEDS; i++) {
        sleds[i].setRGB(0, 0, 0);
        delay(100);
        FastLED.show();
      }
        Serial.println("Nivel 2");
      gameState = 1;
    }
    FastLED.show();
  }
  if (gameState == 1) {
    period = ledSpeed[0];
    if (millis() > time_now + period) {
      time_now = millis();
      if (findRandom) {
        spot = random(56) + 3;
        findRandom = false;
      }
      leds[spot - 1].setRGB(255, 140, 0);
      leds[spot].setRGB(0, 255, 0);
      leds[spot + 1].setRGB(255, 110, 0);
      sleds[0].setRGB(0, 255, 0);
      PlayGame(spot - 1, spot + 1);
    }
    if (digitalRead(A3) == LOW) {
      delay(300);
      findRandom = false;
      if (Position > spot - 1 && Position < spot + 3) {
        level = gameState;
        gameState = 98;
      } else {
        gameState = 99;
      }
    }
  }
  if (gameState == 2) {
//    period = 320;
    period = ledSpeed[1];
    if (millis() > time_now + period) {
      time_now = millis();
      if (findRandom) {
        spot = random(56) + 3;
        findRandom = false;
      }
      leds[spot - 1].setRGB(255, 190, 0);
      leds[spot].setRGB(0, 255, 0);
      leds[spot + 1].setRGB(255, 190, 0);
      sleds[1].setRGB(255, 255, 0);
      PlayGame(spot -1, spot +1);
    }

     if (digitalRead(A3) == LOW) {
      delay(300);
      findRandom = false;
      if (Position > spot - 1 && Position < spot + 3) {
        level = gameState;
        gameState = 98;
      } else {
        gameState = 99;
      }
    }
  }
  
  if (gameState == 3) {
    period = ledSpeed[2];
    if (millis() > time_now + period) {
      time_now = millis();
      if (findRandom) {
        spot = random(56) + 3;
        findRandom = false;
      }
      leds[spot].setRGB(0, 255, 0);
      sleds[2].setRGB(255, 50, 0);
      PlayGame(spot, spot);
    }
    if (digitalRead(A3) == LOW) {
      delay(300);
      if (Position == spot+1) {
        level = gameState;
        gameState = 98;
      } else {
        gameState = 99;
      }
    }
  }
  if (gameState == 4) {
    period = ledSpeed[3];
    if (millis() > time_now + period) {
      time_now = millis();
      if (findRandom) {
        spot = random(56) + 3;
        findRandom = false;
      }
      leds[spot].setRGB(0, 255, 0);
      sleds[3].setRGB(255, 0, 0);
      PlayGame(spot, spot);
    }
    if (digitalRead(A3) == LOW) {
      delay(300);
      if (Position == spot+1) {
        level = gameState;
        gameState = 98;
      } else {
        gameState = 99;
      }
    }
  }

  if (gameState == 5) {
    period = ledSpeed[4];
    if (millis() > time_now + period) {
      time_now = millis();
      if (findRandom) {
        spot = random(56) + 3;
        findRandom = false;
      }
      leds[spot].setRGB(0, 255, 0);
      sleds[4].setRGB(0, 50, 255);
      PlayGame(spot , spot);
    }
    if (digitalRead(A3) == LOW) {
      delay(300);
      if (Position == spot+1) {
        level = gameState;
        gameState = 98;
      } else {
        gameState = 99;
      }
    }
  }

  if (gameState == 6) {
    period = ledSpeed[5];
    if (millis() > time_now + period) {
      time_now = millis();
      if (findRandom) {
        spot = random(56) + 3;
        findRandom = false;
      }
      leds[spot].setRGB(0, 255, 0);
      sleds[5].setRGB(0, 150, 255);
      PlayGame(spot , spot);
    }
    if (digitalRead(A3) == LOW) {
      delay(300);
      if (Position == spot+1) {
        level = gameState;
        gameState = 98;
      } else {
        gameState = 99;
      }
    }
  }
  
  if (gameState == 98) {
    winner();
  }
  if (gameState == 99) {
    loser();
  }
}
void PlayGame(byte bound1, byte bound2) {
  leds[Position].setRGB(255, 0, 0);
  if (Position < bound1 + 1 || Position > bound2 + 1) {
    leds[Position - 1].setRGB(0, 0, 0);
  }
  FastLED.show();
  Position++;
  if (Position >= NUM_LEDS) {
    leds[Position - 1].setRGB(0, 0, 0);
    Position = 0;
  }
}

void winner() {
  for (byte i = 0; i < 3; i++) {
    for (byte j = 0; j < NUM_LEDS; j++) {
      leds[j].setRGB(50, 255, 0);
        tone(9, 1000, 250);
    }
    FastLED.show();
    delay(500);
    clearLEDS();
    FastLED.show();
    delay(500);
  
  }
  findRandom = true;
  Position = 0;

  gameState = level + 1;
  if (gameState > 6) {
    gameState = 0;
  }
}
void loser() {
  for (byte i = 0; i < 3; i++) {
    for (byte j = 0; j < NUM_LEDS; j++) {
      leds[j].setRGB(255, 0, 0);
      tone(9, 200, 250);
    }
    FastLED.show();
    delay(500);
    clearLEDS();
    FastLED.show();
    delay(500);
  }
  gameState = 0;
}
void clearLEDS() {
  for (byte i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
  }
}
void winAll(){
  
}
