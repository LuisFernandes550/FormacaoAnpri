#include "my_defs.h"

int XP=6,XM=A2,YP=A1,YM=7; // IOs para o touch
int TS_LEFT=873,TS_RT=134,TS_TOP=934,TS_BOT=131; 


MCUFRIEND_kbv tft;
//#include <Adafruit_TFTLCD.h>
//Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
uint16_t g_identifier;
extern const uint8_t hanzi[];


TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;
int16_t BOXSIZE;
int16_t PENRADIUS = 1;
uint16_t ID, oldcolor, currentcolor;
uint8_t Orientation = 0;    //PORTRAIT
uint8_t pwm_driver[5]={0,0,0,0,50};

int n_points_temperature=N_POINTS_TEMPERATURE;
int temperature_owen;
int temperature_internal;


int  graph_temperatures[N_POINTS_TIME_AXIS];

int n_current_graph_edit=0;

const char *button_menus_strings[5];


 //Exemplo de curva reflow: https://www.compuphase.com/electronics/reflowsolderprofiles.htm
STRUCT_GRAPH_POINTS graph_refpoints[N_MAX_GRAPHS+1][N_POINTS_TEMPERATURE]={
{ {0,25}, {90,150}, {200,190}, {240,250}, {260,250}, {299,25} },
{ {0,25}, {90,150}, {200,190}, {240,250}, {260,250}, {299,25} },
{ {0,25}, {90,150}, {200,190}, {240,250}, {260,250}, {299,25} },
{ {0,25}, {90,150}, {200,190}, {240,250}, {260,250}, {299,25} }
};



uint8_t icons_top_menu[4];

// Use software SPI: CS, DI, DO, CLK
Adafruit_MAX31865 thermo = Adafruit_MAX31865(PIN_SPI_CS1, PIN_SPI_DI, PIN_SPI_DO, PIN_SPI_CLK);
// use hardware SPI, just pass in the CS pin
//Adafruit_MAX31865 thermo = Adafruit_MAX31865(10)
Adafruit_MAX31865 thermo2 = Adafruit_MAX31865(PIN_SPI_CS2, PIN_SPI_DI, PIN_SPI_DO, PIN_SPI_CLK);

int old_value_encoder,new_value_encoder;
int old_value_push, new_value_push;