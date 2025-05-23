#include <IRremote.hpp>
#include <FastLED.h>
#include "paletas.h"

const byte IR_RECEIVE_PIN = 2;

void setup()
{
   Serial.begin(9600);

   Serial.println("IR receive test");
   IrReceiver.begin(IR_RECEIVE_PIN);
}

void loop()
{
   if (IrReceiver.decode())
   {
      //ignore repeats
      if ((IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT))
      {
         IrReceiver.resume();
         return;
      }      
      unsigned long keycode = IrReceiver.decodedIRData.command;      
      Serial.print("command = ");
      Serial.println(keycode);
      Serial.println();
      
      IrReceiver.resume();
   }
}

