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
extern int temperature_owen;
extern int temperature_internal;

extern int n_current_graph_edit;


extern int  graph_temperatures[N_POINTS_TIME_AXIS];

extern STRUCT_GRAPH_POINTS graph_refpoints[N_MAX_GRAPHS+1][N_POINTS_TEMPERATURE];

extern uint8_t icons_top_menu[4];

extern Adafruit_MAX31865 thermo;
extern Adafruit_MAX31865 thermo2;





void Setup_MAX31865(void) {

  Serial.println("Adafruit MAX31865 PT100 Sensor Test!");

  thermo.begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary 
  thermo2.begin(MAX31865_3WIRE);  // set to 2WIRE or 4WIRE as necessary 
  

}







void Fill_temp_points(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
int16_t steep = abs(y1 - y0) > abs(x1 - x0);
        
    tft.setTextColor(WHITE, BLACK);
   

    if (steep) {
        _swap_int16_t(x0, y0);
        _swap_int16_t(x1, y1);
    }

    if (x0 > x1) {
        _swap_int16_t(x0, x1);
        _swap_int16_t(y0, y1);
    }

    int16_t dx, dy;
    dx = x1 - x0;
    dy = abs(y1 - y0);

    int16_t err = dx / 2;
    int16_t ystep;

    if (y0 < y1) {
        ystep = 1;
    } else {
        ystep = -1;
    }

    for (; x0 <= x1; x0++) {
        if (steep) {
//            tft.writePixel(y0, x0, CYAN);
            graph_temperatures[y0-20]=abs(x0-380);
//             tft.setCursor(2, 400);
//            tft.print("x="+String(y0-20)+"  t="+String(abs(x0-380))+"     ");
//            delay(20);            
        } else {
//            tft.writePixel(x0, y0, CYAN);
            graph_temperatures[x0-20]=abs(y0-380);
//             tft.setCursor(2, 400);
//            tft.print("x="+String(x0-20)+"  t="+String(abs(y0-380))+"     ");
//            delay(20);
        }
        err -= dy;
        if (err < 0) {
        y0 += ystep;
        err += dx;
        }
    }
}




void Update_array_temperatures(int n_graph) {
int i;    
    
    for(i=1;i<n_points_temperature;i++) {
        Fill_temp_points(20+graph_refpoints[n_graph][i-1].time, 380-graph_refpoints[n_graph][i-1].temp, 
                         20+graph_refpoints[n_graph][i].time, 380-graph_refpoints[n_graph][i].temp);
       
    }    
}

void  Test_graph(void) {
int i;


    //Desenha por cima do gráfico para se poder verificar se o que está no array está certo
    tft.setTextColor(WHITE, BLACK);
    tft.setTextSize(2);
    tft.setCursor(2, 415); tft.print("TIME=");
    tft.setCursor(2, 435); tft.print("PROGRAMMED:     ACTUAL:");
    for(i=0;i<N_POINTS_TIME_AXIS;i++) {
        tft.fillCircle(i+20,380-graph_temperatures[i],2,RED);
        tft.setTextColor(TFT_CYAN, BLACK);
        tft.setCursor(65, 415);         tft.print(String(i));
         tft.setTextColor(TFT_CYAN, BLACK);
        tft.setCursor(135, 435);         tft.print(String(graph_temperatures[i]));
        tft.setCursor(280, 435);         tft.print(String(graph_temperatures[i]));
        delay(30);
    }
}


void Main_owen_function(void) {
       
    Update_array_temperatures(n_current_graph_edit);
    Draw_graph(n_current_graph_edit);
    Test_graph();
}