
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






/**************************************************************************/
/*!
   @brief    Write a line.  Bresenham's algorithm - thx wikpedia
    @param    x0  Start point x coordinate
    @param    y0  Start point y coordinate
    @param    x1  End point x coordinate
    @param    y1  End point y coordinate
    @param    color 16-bit 5-6-5 Color to draw with
*/
/**************************************************************************/
void writeLine2(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color) {
int16_t steep = abs(y1 - y0) > abs(x1 - x0);

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
        tft.writePixel(y0, x0, color);
        tft.writePixel(y0-1, x0, color);
        tft.writePixel(y0+1, x0, color);
        tft.writePixel(y0, x0-1, color);
        tft.writePixel(y0, x0+1, color);
        
      
    } else {
        tft.writePixel(x0, y0, color);
        tft.writePixel(x0-1, y0, color);
        tft.writePixel(x0+1, y0, color);
        tft.writePixel(x0, y0-1, color);
        tft.writePixel(x0, y0+1, color);
        
        
    }
    err -= dy;
    if (err < 0) {
      y0 += ystep;
      err += dx;
    }
  }
}





void draw_icon(int x, uint8_t n_icon) {
   
    x=x*(tft.width()/4);

    switch(n_icon) {
        case ICON_NULL: break;
        case ICON_HOME: writeLine2(x+40,5,x+20,18,TFT_WHITE);
                        writeLine2(x+40,5,x+60,18,TFT_WHITE);
                        writeLine2(x+25,15,x+25,40,TFT_WHITE);
                        writeLine2(x+55,15,x+55,40,TFT_WHITE);   
                        writeLine2(x+25,40,x+36,40,TFT_WHITE);
                        writeLine2(x+36,40,x+36,25,TFT_WHITE);
                        writeLine2(x+55,40,x+44,40,TFT_WHITE);
                        writeLine2(x+44,40,x+44,25,TFT_WHITE);
                        writeLine2(x+36,25,x+44,25,TFT_WHITE);
                        return;

        case ICON_LEFT_ARROW: writeLine2(x+25,25,x+55,25,WHITE);
                        writeLine2(x+25,25,x+35,15,WHITE);
                        writeLine2(x+25,25,x+35,35,WHITE);
                        return;
        
        case ICON_RIGHT_ARROW: writeLine2(x+25,25,x+55,25,WHITE);
                        writeLine2(x+55,25,x+45,15,WHITE);
                        writeLine2(x+55,25,x+45,35,WHITE);
                        return;

        case ICON_NEXT: tft.setTextSize(2);
                        tft.setTextColor(WHITE, BAR_MENU_COLOR );    
                        tft.setCursor(x+20, 17); 
                        tft.print("NEXT"); 
                        break;

                        

        case ICON_OK:   writeLine2(x+30,25,x+35,40,WHITE);
                        writeLine2(x+35,40,x+65,10,WHITE);
                        return;

        case ICON_CANCEL: writeLine2(x+25,15,x+45,35,WHITE);
                          writeLine2(x+25,35,x+45,15,WHITE);
                          return;

    }
}


void draw_top_bar_menu(void) {
uint8_t i;

    tft.fillRect(0, 0, tft.width()-1, 50,  BAR_MENU_COLOR); 

    for( i=0;i<4;i++) {
        writeLine2(i*tft.width()/4,0,i*tft.width()/4,50,TFT_DARKCYAN); 
        draw_icon(i, icons_top_menu[i]);
    }
    writeLine2(i*tft.width()/4,0,i*tft.width()/4,50,TFT_DARKCYAN); 
}
    





void Draw_graph(uint8_t n_graph) {
int i;

    tft.fillRect(0, 50, 320, 480-51, BLACK);

    tft.setTextColor(WHITE, BLACK);
    tft.setTextSize(1);
    tft.setCursor(2, 85);
    tft.print("temp[C]");
    tft.drawLine(20,100,20,380,GREEN);
    tft.setCursor(274, 400);
    tft.print("time[s]");
    tft.drawLine(20,380,20+N_POINTS_TIME_AXIS,380,GREEN);

    tft.setTextColor(TFT_LIGHTGREY, BLACK);
    tft.setCursor(15, 390);
    tft.print("0");
    


    
    for(i=0;i<11;i++) {
        if(i!=10) {
            tft.setCursor(0, 100+280/10*i-2);
            tft.print(String((10-i)*280/10) );
        }
        tft.drawLine(15,100+280/10*i,25,100+280/10*i,TFT_GREENYELLOW);
        
  
        if (i!=10) {
            tft.drawLine(20+300/10*i,375,20+300/10*i,385,TFT_GREENYELLOW);
            if(i!=0) {
                tft.setCursor(20+300/10*i-5,390);
                tft.print(String((i)*300/10));
            }    
        }  
        else
            tft.drawLine(20+300/10*i-1,375,20+300/10*i-1,385,TFT_GREENYELLOW);
        

    }
    
    tft.setTextSize(20);
    tft.setTextColor(GRAPH_WATEMARK_COLOR, BLACK );
    tft.setCursor(120, 150); tft.print(n_current_graph_edit); 
    


    for(i=0;i<n_points_temperature;i++) {
        if (i>0) {

           tft.drawLine(20+graph_refpoints[n_graph][i-1].time, 380-graph_refpoints[n_graph][i-1].temp, 
                        20+graph_refpoints[n_graph][i].time, 380-graph_refpoints[n_graph][i].temp, TFT_CYAN);
        }
        tft.fillCircle(20+graph_refpoints[n_graph][i].time, 380-graph_refpoints[n_graph][i].temp, 4, TFT_CYAN);     
    }
        
}


