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



void Edit_graph(void) {
uint16_t xpos=5000, ypos=5000;  //screen coordinates (ypos=5000 para n detectar top bar menu)
int i;
int  need_redraw=FALSE;
uint8_t menu_option_selected;
    
    icons_top_menu[0]=ICON_HOME;  icons_top_menu[1]=ICON_NEXT; icons_top_menu[2]=ICON_OK; icons_top_menu[3]=ICON_CANCEL;
    draw_top_bar_menu();


    Draw_graph(n_current_graph_edit);

    for (i=0;i<N_POINTS_TEMPERATURE;i++) {
        
        graph_refpoints[N_MAX_GRAPHS][i].temp=graph_refpoints[n_current_graph_edit][i].temp;
        graph_refpoints[N_MAX_GRAPHS][i].time=graph_refpoints[n_current_graph_edit][i].time;
    }

    for(;;) {
        tp = ts.getPoint();   //tp.x, tp.y are ADC values

        // if sharing pins, you'll need to fix the directions of the touchscreen pins
        pinMode(XM, OUTPUT);
        pinMode(YP, OUTPUT);
        // we have some minimum pressure we consider 'valid'
        // pressure of 0 means no pressing!

        if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
            // most mcufriend have touch (with icons) that extends below the TFT
            // screens without icons need to reserve a space for "erase"
            // scale the ADC values from ts.getPoint() to screen values e.g. 0-239
            //
            // Calibration is true for PORTRAIT. tp.y is always long dimension 
            // map to your current pixel orientation
            switch (Orientation) {
                case 0:
                    xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
                    ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
                    break;
                case 1:
                    xpos = map(tp.y, TS_TOP, TS_BOT, 0, tft.width());
                    ypos = map(tp.x, TS_RT, TS_LEFT, 0, tft.height());
                    break;
                case 2:
                    xpos = map(tp.x, TS_RT, TS_LEFT, 0, tft.width());
                    ypos = map(tp.y, TS_BOT, TS_TOP, 0, tft.height());
                    break;
                case 3:
                    xpos = map(tp.y, TS_BOT, TS_TOP, 0, tft.width());
                    ypos = map(tp.x, TS_LEFT, TS_RT, 0, tft.height());
                    break;
            }

            for(i=1;i<n_points_temperature;i++) {
                if (xpos<20+graph_refpoints[N_MAX_GRAPHS][i].time+DX && xpos>20+graph_refpoints[N_MAX_GRAPHS][i].time-DACC_MAX_DATA)
                    if (ypos<380-graph_refpoints[N_MAX_GRAPHS][i].temp+DY && ypos>380-graph_refpoints[N_MAX_GRAPHS][i].temp-DY)
                        break;
            }
            if (i!=n_points_temperature) {
                tft.drawLine(20+graph_refpoints[N_MAX_GRAPHS][i-1].time, 380-graph_refpoints[N_MAX_GRAPHS][i-1].temp, 20+graph_refpoints[N_MAX_GRAPHS][i].time, 380-graph_refpoints[N_MAX_GRAPHS][i].temp, BLACK);
                tft.drawLine(20+graph_refpoints[N_MAX_GRAPHS][i].time, 380-graph_refpoints[N_MAX_GRAPHS][i].temp, 20+graph_refpoints[N_MAX_GRAPHS][i+1].time, 380-graph_refpoints[N_MAX_GRAPHS][i+1].temp, BLACK);
                tft.fillCircle(20+graph_refpoints[N_MAX_GRAPHS][i].time, 380-graph_refpoints[N_MAX_GRAPHS][i].temp, 4, BLACK);  
              
                graph_refpoints[N_MAX_GRAPHS][i].time=xpos-20;
                graph_refpoints[N_MAX_GRAPHS][i].temp=380-ypos;

                tft.setTextColor(WHITE, BLACK);
                tft.setTextSize(2);
                tft.setCursor(32, 420); tft.print("TIME="+String(graph_refpoints[N_MAX_GRAPHS][i].time)+" ");
                tft.setCursor(160, 420); tft.print("TEMP["+String(i)+"]="+String(graph_refpoints[N_MAX_GRAPHS][i].temp)+" ");
                tft.setTextColor(WHITE, BLACK);

                tft.drawLine(20+graph_refpoints[N_MAX_GRAPHS][i-1].time, 380-graph_refpoints[N_MAX_GRAPHS][i-1].temp, 20+graph_refpoints[N_MAX_GRAPHS][i].time, 380-graph_refpoints[N_MAX_GRAPHS][i].temp, TFT_CYAN);
                if (i!=n_points_temperature-1) 
                    tft.drawLine(20+graph_refpoints[N_MAX_GRAPHS][i].time, 380-graph_refpoints[N_MAX_GRAPHS][i].temp, 20+graph_refpoints[N_MAX_GRAPHS][i+1].time, 380-graph_refpoints[N_MAX_GRAPHS][i+1].temp, TFT_CYAN);
                tft.fillCircle(20+graph_refpoints[N_MAX_GRAPHS][i].time, 380-graph_refpoints[N_MAX_GRAPHS][i].temp, 4, TFT_RED);  
                need_redraw=TRUE;
            }
        }
        else {
            if (need_redraw) {
                Draw_graph(N_MAX_GRAPHS);
                need_redraw=FALSE;
            }
            if (ypos<50) {
                menu_option_selected=xpos/(tft.width()/4);

                tft.fillRect(1+menu_option_selected*(tft.width()/4), 0, (tft.width()/4)-2, 50, RED); 
                draw_icon(menu_option_selected, icons_top_menu[menu_option_selected]);
                delay(50);
                switch(menu_option_selected) {
                    case 0: Update_array_temperatures(n_current_graph_edit);
                            //Test_graph();
                            return;

                    case 1: if (n_current_graph_edit!=N_MAX_GRAPHS-1)
                                n_current_graph_edit++;
                            else
                                n_current_graph_edit=0;
                            for (i=0;i<N_POINTS_TEMPERATURE;i++) {
                                graph_refpoints[N_MAX_GRAPHS][i].temp=graph_refpoints[n_current_graph_edit][i].temp;
                                graph_refpoints[N_MAX_GRAPHS][i].time=graph_refpoints[n_current_graph_edit][i].time;
                            }    
                            Draw_graph(N_MAX_GRAPHS);
                            need_redraw=FALSE; 

                            break;     
                    case 2: for (i=0;i<N_POINTS_TEMPERATURE;i++) {
                                graph_refpoints[n_current_graph_edit][i].temp=graph_refpoints[N_MAX_GRAPHS][i].temp;
                                graph_refpoints[n_current_graph_edit][i].time=graph_refpoints[N_MAX_GRAPHS][i].time;
                            }
                            break;    
                    
                    case 3:  for (i=0;i<N_POINTS_TEMPERATURE;i++) {
                                graph_refpoints[N_MAX_GRAPHS][i].temp=graph_refpoints[n_current_graph_edit][i].temp;
                                graph_refpoints[N_MAX_GRAPHS][i].time=graph_refpoints[n_current_graph_edit][i].time;
                            }
                            Draw_graph(N_MAX_GRAPHS);
                            need_redraw=FALSE; 
                            break;   
                }
                tft.fillRect(1+menu_option_selected*(tft.width()/4), 0, (tft.width()/4)-2, 50, BAR_MENU_COLOR); 
                draw_icon(menu_option_selected, icons_top_menu[menu_option_selected]);
                delay(50);
                ypos=5000;
            }

        }

    }



}