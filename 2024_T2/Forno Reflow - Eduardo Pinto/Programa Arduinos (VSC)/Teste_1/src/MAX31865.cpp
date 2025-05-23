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

extern STRUCT_GRAPH_POINTS graph_refpoints[N_MAX_GRAPHS+1][N_POINTS_TEMPERATURE];

extern uint8_t icons_top_menu[4];

extern uint8_t n_current_graph_edit;

extern Adafruit_MAX31865 thermo;
extern Adafruit_MAX31865 thermo2;

extern const char *button_menus_strings[5];

extern int temperature_owen;
extern int temperature_internal;


void Test_MAX31865(void) {
uint16_t rtd;
float ratio;
uint8_t fault;
    
   
/*    
    //Para testar os sinais nos pinos com o osciloscópio
    for(;;) {
        thermo.writeRegister8(0xF0, 0x12);
        delay(10);
    }
 */   
    for(;;) {
        tft.fillScreen(BLACK);
        tft.setTextColor(WHITE, BLACK);
        tft.setTextSize(2);

        //Sensor de temperatura 1 - Temperatura interna do forno
        rtd = thermo.readRTD();
        tft.setCursor(10, 50); tft.print("RTD value: "+String(rtd)+"  ");
        Serial.print("RTD value: "); Serial.println(rtd);
    
        ratio=rtd;
        ratio /= 32768;
        tft.setCursor(10, 70); tft.print("Ratio = "+String(ratio)+"  ");
        Serial.print("Ratio = "); Serial.println(ratio,8);
        tft.setCursor(10, 90); tft.print("Resistance = "+String(RREF*ratio)+"  ");
        Serial.print("Resistance = "); Serial.println(RREF*ratio,8);
        tft.setCursor(10, 110); tft.print("Temperature = "+String(thermo.temperature(RNOMINAL, RREF))+"  ");
        Serial.print("Temperature = "); Serial.println(thermo.temperature(RNOMINAL, RREF));


        // Check and print any faults
        fault = thermo.readFault();
        if (fault) {
            tft.setTextSize(2);
            tft.setCursor(100, 200); tft.print("Fault 0x"+String(fault)+"  ");
            tft.setTextColor(RED, BLACK);
            tft.setTextSize(2);
            tft.setCursor(0, 220);
            if (fault & MAX31865_FAULT_HIGHTHRESH) {
                tft.print("RTD1 High Threshold      ");
                Serial.println("RTD High Threshold"); 
            }
            if (fault & MAX31865_FAULT_LOWTHRESH) {
                tft.print("RTD1 Low Threshold      ");
                Serial.println("RTD Low Threshold"); 
            }
            if (fault & MAX31865_FAULT_OVUV) {
                tft.print("RTD1 Under/Over voltage");
                Serial.println("RTD1 Under/Over voltage"); 
            }
            tft.setCursor(0, 240);
            if (fault & MAX31865_FAULT_REFINLOW) {
                tft.print("RTD1 REFIN- > 0.85 x Bias      ");
                Serial.println("RTD1 REFIN- > 0.85 x Bias"); 
            }
            tft.setCursor(0, 260);
            if (fault & MAX31865_FAULT_REFINHIGH) {
                tft.print("RTD1 REFIN- < 0.85 x Bias - FORCE- open");
                Serial.println("RTD1 REFIN- < 0.85 x Bias - FORCE- open"); 
            }
            tft.setCursor(0, 280);
            if (fault & MAX31865_FAULT_RTDINLOW) {
                tft.print("RTD1 RTDIN- < 0.85 x Bias - FORCE- open");
                Serial.println("RTD1 RTDIN- < 0.85 x Bias - FORCE- open"); 
            }
            
            thermo.clearFault();
            delay(1000);
        }    


        //Sensor de temperatura 2 - Temperatura interna
        tft.setTextColor(WHITE, BLACK);
        rtd = thermo2.readRTD();
        tft.setCursor(10, 150); tft.print("RTD2 value: "+String(rtd)+"  ");
        Serial.print("RTD2 value: "); Serial.println(rtd);
    
        ratio=rtd;
        ratio /= 32768;
        tft.setCursor(10, 170); tft.print("Ratio2 = "+String(ratio)+"  ");
        Serial.print("Ratio2 = "); Serial.println(ratio,8);
        tft.setCursor(10, 190); tft.print("Resistance2 = "+String(RREF*ratio)+"  ");
        Serial.print("Resistance2 = "); Serial.println(RREF*ratio,8);
        tft.setCursor(10, 210); tft.print("Temperature2 = "+String(thermo2.temperature(RNOMINAL, RREF))+"  ");
        Serial.print("Temperature2 = "); Serial.println(thermo2.temperature(RNOMINAL, RREF));


        // Check and print any faults
        fault = thermo2.readFault();
        if (fault) {
            tft.setTextSize(2);
            tft.setCursor(100, 300); tft.print("Fault 0x"+String(fault)+"  ");
            tft.setTextColor(RED, BLACK);
            tft.setTextSize(2);
            tft.setCursor(0, 340);
            if (fault & MAX31865_FAULT_HIGHTHRESH) {
                tft.print("RTD2 High Threshold      ");
                Serial.println("RTD2 High Threshold"); 
            }
            if (fault & MAX31865_FAULT_LOWTHRESH) {
                tft.print("RTD2 Low Threshold      ");
                Serial.println("RTD2 Low Threshold"); 
            }
            if (fault & MAX31865_FAULT_OVUV) {
                tft.print("RTD2 Under/Over voltage");
                Serial.println("RTD2 Under/Over voltage"); 
            }
            tft.setCursor(0, 360);
            if (fault & MAX31865_FAULT_REFINLOW) {
                tft.print("RTD2 REFIN- > 0.85 x Bias      ");
                Serial.println("RTD2 REFIN- > 0.85 x Bias"); 
            }
            tft.setCursor(0, 380);
            if (fault & MAX31865_FAULT_REFINHIGH) {
                tft.print("RTD2 REFIN- < 0.85 x Bias - FORCE- open");
                Serial.println("RTD2 REFIN- < 0.85 x Bias - FORCE- open"); 
            }
            tft.setCursor(0, 400);
            if (fault & MAX31865_FAULT_RTDINLOW) {
                tft.print("RTD2 RTDIN- < 0.85 x Bias - FORCE- open");
                Serial.println("RTD2 RTDIN- < 0.85 x Bias - FORCE- open"); 
            }

            thermo2.clearFault();
        }
        Serial.println();
        delay(500);

    }
}

uint8_t Read_temperature_owen(void) {
    
    temperature_owen=(int)thermo.temperature(RNOMINAL, RREF);
    uint8_t fault = thermo.readFault();
    if (fault)
        temperature_owen=0;
    
    return fault;
}   

uint8_t Read_temperature_internal(void) {
    
    temperature_owen=(int)thermo2.temperature(RNOMINAL, RREF);
    uint8_t fault = thermo2.readFault();
    if (fault)
        temperature_owen=0;
    
    return fault;
}
