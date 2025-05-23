#include "my_defs.h"

extern int old_value_encoder,new_value_encoder;
extern int old_value_push, new_value_push;

void Setup_Encoder(void) {

    pinMode(PIN_ENCODER_INPUT_A,INPUT_PULLUP);
    
    pinMode(PIN_ENCODER_INPUT_B,INPUT_PULLUP);
    pinMode(PIN_ENCODER_INPUT_PUSH,INPUT_PULLUP);
    new_value_encoder=(digitalRead(PIN_ENCODER_INPUT_A)<<1) + digitalRead(PIN_ENCODER_INPUT_B);
    old_value_push=1;
}


//Algoritmo adaptado de https://cdn.sparkfun.com/datasheets/Robotics/How%20to%20use%20a%20quadrature%20encoder.pdf
uint8_t Read_Encoder(void) {
int Encoder_inputA,Encoder_inputB;
uint8_t Out;
int QEM [4][4] = {
    {STEADY , LEFT   , RIGHT  , ERROR },
    {RIGHT  , STEADY , ERROR  , LEFT  },
    {LEFT   , ERROR  , STEADY , RIGHT },
    {ERROR  , RIGHT  , LEFT   , STEADY}
};
  
    //Serial.println("Reading Encoder 62A!");

    Encoder_inputA=digitalRead(PIN_ENCODER_INPUT_A);
    Encoder_inputB=digitalRead(PIN_ENCODER_INPUT_B);
    new_value_push=digitalRead(PIN_ENCODER_INPUT_PUSH);
    delay(2); //Debouncing. Especialmente importante para o botão de PUSH

    
    old_value_encoder=new_value_encoder;
    new_value_encoder=(Encoder_inputA<<1) + Encoder_inputB;
    Out = QEM [old_value_encoder][new_value_encoder];
    switch(Out) {
        case STEADY://Serial.println("STEADY");
                break;
        case LEFT:Serial.println("LEFT");
                break;
        case RIGHT:Serial.println("RIGHT");
                break;

        case ERROR: Serial.println("ERROR!");
                break; 
    }
    if (old_value_push!=new_value_push) {
        old_value_push=new_value_push;
        if (new_value_push==0) {
            Out|=PUSH;    
            Serial.println("PUSH");
        } 
           
    }    
    
    return Out;
}
