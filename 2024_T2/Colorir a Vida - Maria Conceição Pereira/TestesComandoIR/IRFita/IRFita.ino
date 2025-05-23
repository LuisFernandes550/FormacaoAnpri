#include <IRremote.hpp>

const byte IR_RECEIVE_PIN = 2;
const byte redLedPin = 12;
const byte greenLedPin = 13;
const byte yellowLedPin = 8;

unsigned long keycode;
bool newCode = false; 
// new entry flag

void setup()
{
   Serial.begin(9600);

   pinMode(redLedPin, OUTPUT);
   pinMode(greenLedPin, OUTPUT);
   pinMode(yellowLedPin, OUTPUT);

   Serial.println("IR receive test");
   IrReceiver.begin(IR_RECEIVE_PIN);
}

void loop()
{
   getRemoteCode();

   if (newCode == true)  // is there a new keycode?
   {
      if (keycode == 66)
      {
          Serial.print("GREEN");
          digitalWrite(greenLedPin, HIGH);
      }
      else if (keycode == 82)
      {
          Serial.print("RED");
          digitalWrite(redLedPin, HIGH);
      }
      else if (keycode == 74)
      {
        Serial.print("YELLOW");
        digitalWrite(yellowLedPin, HIGH);
      }
      else
      {
        Serial.print("invalid code");
        digitalWrite(redLedPin, LOW);
        digitalWrite(greenLedPin, LOW);
        digitalWrite(yellowLedPin, LOW);
      }
      newCode = false;  // reset flag
   }
}

void getRemoteCode()
{
   if (IrReceiver.decode())
   {
      //ignore repeats
      if ((IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT))
      {
         IrReceiver.resume();
         return;
      }
      keycode = IrReceiver.decodedIRData.command;
      Serial.print("command = ");
      Serial.println(keycode);

      IrReceiver.resume();
      newCode = true; // set new entry flag
   }
}