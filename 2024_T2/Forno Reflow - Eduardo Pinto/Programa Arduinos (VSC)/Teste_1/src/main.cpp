#include "my_defs.h"


extern int XP,XM,YP,YM; // IOs para o touch
extern int TS_LEFT,TS_RT,TS_TOP,TS_BOT; 

extern MCUFRIEND_kbv tft;
extern uint16_t g_identifier;
extern const uint8_t hanzi[];

extern TouchScreen ts; 
extern TSPoint tp;
extern int16_t BOXSIZE;
extern int16_t PENRADIUS;
extern uint16_t ID, oldcolor, currentcolor;
extern uint8_t Orientation;    //PORTRAIT
extern uint8_t pwm_driver[5];

extern int n_points_temperature;

extern int  graph_temperatures[N_POINTS_TIME_AXIS];
extern int temperature_owen;
extern STRUCT_GRAPH_POINTS graph_refpoints[N_MAX_GRAPHS+1][N_POINTS_TEMPERATURE];

extern uint8_t icons_top_menu[4];


void setup(void) {
    
    Serial.begin(115200);
 /*
    //Para testar com o osciloscópio um pino
    pinMode(PIN_SPI_CS1,OUTPUT);
    for(;;) {
        digitalWrite(PIN_SPI_CS1,1);
        Serial.println(1);
        delay(1);
        digitalWrite(PIN_SPI_CS1,0);
        Serial.println(0);
        delay(1);
    }
*/

    Setup_PWM(); 

    Setup_MAX31865();

    Setup_Encoder();

    //Setup the output pins for relays 1..3    
    pinMode(PIN_RELAY1_OUTPUT , OUTPUT);
    pinMode(PIN_RELAY2_OUTPUT , OUTPUT);
    pinMode(PIN_RELAY3_OUTPUT , OUTPUT);

    //Setup the pins for quick acess buttons  
    pinMode(PIN_BUTTON1,INPUT_PULLUP);  
    pinMode(PIN_BUTTON2,INPUT_PULLUP);  
    pinMode(PIN_BUTTON3,INPUT_PULLUP);  
    pinMode(PIN_BUTTON4,INPUT_PULLUP);  

    
    
    uint32_t when = millis();
    //    while (!Serial) ;   //hangs a Leonardo until you connect a Serial
    if (!Serial) delay(5000);           //allow some time for Leonardo
    Serial.println("Serial took " + String((millis() - when)) + "ms to start");
    //    tft.reset();                 //hardware reset
    uint16_t ID = tft.readID(); //
    //ID=0x9481; //No primeiro de tudos tive que forçar para o 0x9481 (os dois novos são 0x9486)
    Serial.print("ID = 0x");
    Serial.println(ID, HEX);
    if (ID == 0xD3D3) ID = 0x9481; // write-only shield
//    ID = 0x9329;                             // force ID
    tft.begin(ID);
    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);
   
    tft.setTextColor(WHITE, BLACK);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.print("WIDTH="+String(tft.width()));
    tft.print("HEIGHT="+String(tft.height()));
    delay(1000);

    for(int i=0;i<N_MAX_GRAPHS;i++ ) {
    
    if (!Read_temperature_owen())
        graph_refpoints[i][0].temp=temperature_owen;
    else    
        graph_refpoints[i][0].temp=0;
}
 
  
}












void loop(void) {
    
    //Test_MAX31865();
    //Test_Encoder();
    //Test_Read_Encoder_function();
    //Test_buttons();



    Main_menu();

}