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

extern const char *button_menus_strings[5];


void Test_Outputs(void) {
uint16_t xpos=5000, ypos=5000;  //screen coordinates (ypos=5000 para n detectar top bar menu)
int n_option_menu_active=0; //-1 significa nenhum activo, 0=primeiro opção menu, 2=Segunda...
int n_options_menu;
int i;

    n_options_menu=3;
    button_menus_strings[0]="R1 INT. FAN";
    button_menus_strings[1]="R2 OWEN FAN";
    button_menus_strings[2]="R3 LIGHT";
    button_menus_strings[3]="MAIN MENU";
    button_menus_strings[4]="";
    Draw_bar_menus(TRUE, n_option_menu_active,-1);
    
    tft.setTextSize(2);
    tft.setCursor(50, 400);
    tft.setTextColor(WHITE,BLACK);
    tft.print("R1=" + String(digitalRead(PIN_RELAY1_OUTPUT)) + "  R2=" + String(digitalRead(PIN_RELAY2_OUTPUT)) + "  R3=" +  String(digitalRead(PIN_RELAY3_OUTPUT)) );
   
    for(;;) {

        uint8_t enc=Read_Encoder();

        if (enc&PUSH) {
            Draw_bar_menus(FALSE, -1 ,n_option_menu_active); 
            delay(200);
            switch(n_option_menu_active) {

                case 0: digitalWrite(PIN_RELAY1_OUTPUT, !digitalRead(PIN_RELAY1_OUTPUT));  // toggle PIN_RELAY1_OUTPUT
                        break;
                case 1: digitalWrite(PIN_RELAY2_OUTPUT, !digitalRead(PIN_RELAY2_OUTPUT));  // toggle PIN_RELAY1_OUTPUT
                        break;
                case 2: digitalWrite(PIN_RELAY3_OUTPUT, !digitalRead(PIN_RELAY3_OUTPUT));  // toggle PIN_RELAY1_OUTPUT
                        break;
                case 3: return;        
                
            }
            tft.setTextSize(2);
            tft.setCursor(50, 400);
            tft.setTextColor(WHITE,BLACK);
            tft.print("R1=" + String(digitalRead(PIN_RELAY1_OUTPUT)) + "  R2=" + String(digitalRead(PIN_RELAY2_OUTPUT)) + "  R3=" +  String(digitalRead(PIN_RELAY3_OUTPUT)) );
   
            Draw_bar_menus(FALSE, n_option_menu_active ,-1); 
            delay(200);
          
        }
        else {
        
            switch(enc) {
                case STEADY: break;
                case LEFT:  if (n_option_menu_active==0)
                                n_option_menu_active=n_options_menu-1;
                            else
                                n_option_menu_active--;
                            Draw_bar_menus(FALSE, n_option_menu_active,-1);
                            break;

                case RIGHT: if (n_option_menu_active==n_options_menu-1)
                                n_option_menu_active=0;
                            else
                                n_option_menu_active++;
                            Draw_bar_menus(FALSE, n_option_menu_active,-1);
                            break;

                case ERROR:break;

            }
        }

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

            for(i=0;i<5;i++) {
                if (strlen(button_menus_strings[i])){
                    if (xpos>50 && xpos<270) 
                        if (ypos>50+i*80 && ypos<50+i*80+50)
                            break;
                }
            }
            if (i!=5) {
                n_option_menu_active=i;
                Draw_bar_menus(FALSE, -1 ,n_option_menu_active); 
                delay(200);
                switch(i) {
                    case 0: digitalWrite(PIN_RELAY1_OUTPUT, !digitalRead(PIN_RELAY1_OUTPUT));  // toggle PIN_RELAY1_OUTPUT
                            break;
                    case 1: digitalWrite(PIN_RELAY2_OUTPUT, !digitalRead(PIN_RELAY2_OUTPUT));  // toggle PIN_RELAY1_OUTPUT
                            break;
                    case 2: digitalWrite(PIN_RELAY3_OUTPUT, !digitalRead(PIN_RELAY3_OUTPUT));  // toggle PIN_RELAY1_OUTPUT
                            break;
                    case 3: return;   
                }
                Draw_bar_menus(FALSE, n_option_menu_active, -1); 
            }
            tft.setTextSize(2);
            tft.setCursor(50, 400);
            tft.setTextColor(WHITE,BLACK);
            tft.print("R1=" + String(digitalRead(PIN_RELAY1_OUTPUT)) + "  R2=" + String(digitalRead(PIN_RELAY2_OUTPUT)) + "  R3=" +  String(digitalRead(PIN_RELAY3_OUTPUT)) );
   
        }                   
    }


}

void Test_PWM(void) {
uint16_t xpos=5000, ypos=5000;  //screen coordinates
uint8_t pwm_bar_selected=0;
char str_aux[10];
uint8_t menu_option_selected;
uint8_t aux_pwm_driver[5];

    tft.setTextColor(WHITE, BLACK);
    tft.setTextSize(2);
    tft.fillScreen(BLACK);

    icons_top_menu[0]=ICON_HOME; icons_top_menu[1]=ICON_LEFT_ARROW; icons_top_menu[2]=ICON_OK;icons_top_menu[3]=ICON_CANCEL;
    draw_top_bar_menu();

    //tft.width()=320   tft.height=480
    BOXSIZE = tft.width() / 5-5;

    for(uint8_t i=0; i<5; i++)  {
        aux_pwm_driver[i]=pwm_driver[i];
        tft.fillRect((BOXSIZE+5)*i+1, tft.height()-pwm_driver[i]*3-10+1, BOXSIZE-2, pwm_driver[i]*3-2 , TFT_WHITE);
        tft.drawRect((BOXSIZE+5)*i, tft.height()-100*3-10-2, BOXSIZE, 100*3+2, TFT_GREEN);
    }

    for(;;) {
        for(uint8_t i=0; i<5; i++)  {
            tft.setCursor((BOXSIZE+5)*i, 150);
            sprintf(str_aux,"%3d%%",(pwm_driver[i]));
            tft.print(str_aux);
        }    
   
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

            if (ypos<50) {
                menu_option_selected=xpos/(tft.width()/4);

                tft.fillRect(1+menu_option_selected*(tft.width()/4), 0, (tft.width()/4)-2, 50, RED); 
                draw_icon(menu_option_selected, icons_top_menu[menu_option_selected]);
                delay(500);
                switch(menu_option_selected) {
                    case 0: 
                    case 1: 
                    case 2:       return; 
                    
                    case 3:   for(uint8_t i=0; i<5; i++) {
                                            pwm_driver[i]=aux_pwm_driver[i];
                                            Set_PWM_channel(i);
                                        }   
                                        return;
                }
            }
            else {
      
                if (ypos>=tft.height()-100*3-10 && ypos<tft.height()-11) {
                    pwm_bar_selected=xpos/(BOXSIZE+5);
                    pwm_driver[pwm_bar_selected]=((tft.height()-10)-ypos)/3;
                    tft.drawRect((BOXSIZE+5)*pwm_bar_selected, tft.height()-100*3-10-2, BOXSIZE, 100*3+2, TFT_RED);
                    tft.fillRect((BOXSIZE+5)*pwm_bar_selected+1, tft.height()-pwm_driver[pwm_bar_selected]*3-10-1, BOXSIZE-1, pwm_driver[pwm_bar_selected]*3 ,  TFT_RED);
                    tft.fillRect((BOXSIZE+5)*pwm_bar_selected+1, tft.height()-100*3-10-1, BOXSIZE-2, (100-pwm_driver[pwm_bar_selected])*3-1 , BLACK);
                    Set_PWM_channel(pwm_bar_selected);                 
                }
         
            }  
        }
        else {   
            tft.drawRect((BOXSIZE+5)*pwm_bar_selected, tft.height()-100*3-10-2, BOXSIZE, 100*3+2, TFT_GREEN);
            tft.fillRect((BOXSIZE+5)*pwm_bar_selected+1, tft.height()-pwm_driver[pwm_bar_selected]*3-10-1, BOXSIZE-1, pwm_driver[pwm_bar_selected]*3 , TFT_WHITE);
            if (pwm_driver[pwm_bar_selected]!=100 && pwm_driver[pwm_bar_selected]!=0)
                tft.fillRect((BOXSIZE+5)*pwm_bar_selected+1, tft.height()-100*3-10-1, BOXSIZE-2, (101-pwm_driver[pwm_bar_selected])*3-1 , BLACK);
          
        }      
    }
}    

//Algoritmo adaptado de https://cdn.sparkfun.com/datasheets/Robotics/How%20to%20use%20a%20quadrature%20encoder.pdf
void Test_Encoder(void) {
int Encoder_oldinputA=-1,Encoder_oldinputB=-1;
int Encoder_inputA,Encoder_inputB,Encoder_inputPUSH;
bool Encoder_rotation_done=FALSE;
bool printed_push=FALSE;
int QEM [4][4] = {
    {STEADY , LEFT   , RIGHT  , ERROR },
    {RIGHT  , STEADY , ERROR  , LEFT  },
    {LEFT   , ERROR  , STEADY , RIGHT },
    {ERROR  , RIGHT  , LEFT   , STEADY}
};
int Old,Out, New=0; 

    
    Serial.println("Optical Encoder 62A Test!");
    Encoder_inputA=digitalRead(PIN_ENCODER_INPUT_A);
    Encoder_inputB=digitalRead(PIN_ENCODER_INPUT_B);
    New=(Encoder_inputA<<1) + Encoder_inputB;

    for(;;) {
        Encoder_inputA=digitalRead(PIN_ENCODER_INPUT_A);
        Encoder_inputB=digitalRead(PIN_ENCODER_INPUT_B);
        Encoder_inputPUSH=digitalRead(PIN_ENCODER_INPUT_PUSH);
        Serial.print("INPUT_A= ");  Serial.println(Encoder_inputA);
        Serial.print("INPUT_B= ");  Serial.println(Encoder_inputB);
        Serial.print("INPUT_P= ");  Serial.println(Encoder_inputPUSH);
        Serial.println();
        delay(500);


        Old=New;
        New=(Encoder_inputA<<1) + Encoder_inputB;
        Out = QEM [Old][New];
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
        if (Encoder_inputPUSH==0) {
            if (!printed_push)
                Serial.println("PUSH");
            printed_push=TRUE;
            delay(1);//Debouncing
        }
        else  {
            printed_push=FALSE;
        }
    }

    //Testes basicos
    for(;;) {
        Encoder_inputA=digitalRead(22);
        Encoder_inputB=digitalRead(24);
        Encoder_inputPUSH=digitalRead(26);
        Encoder_rotation_done=FALSE;

        if (Encoder_inputA!=Encoder_oldinputA) {
            Encoder_oldinputA=Encoder_inputA;
            Encoder_rotation_done=TRUE;
        }
        if (Encoder_inputB!=Encoder_oldinputB) {
            Encoder_oldinputB=Encoder_inputB;
            Encoder_rotation_done=TRUE;
        }

        if (Encoder_rotation_done) {
            Serial.print("A="); Serial.print(Encoder_inputA); Serial.print("  ,B="); Serial.println(Encoder_inputB);
            Serial.println("");
         
        }
        if (Encoder_inputPUSH==0) {
            if (!printed_push)
                Serial.println("PUSH PRESSED!");
            printed_push=TRUE;
        }
        else  {
            printed_push=FALSE;
        }

         
         delay(100);
    }
}



void Test_hand_drawing(void) {
uint16_t xpos=5000, ypos=5000;  //screen coordinates
    
    BOXSIZE = tft.width() / 6;
    tft.fillScreen(BLACK);

    tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
    tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
    tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, GREEN);
    tft.fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, CYAN);
    tft.fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, BLUE);
    tft.fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, MAGENTA);

    tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
    currentcolor = RED;
    delay(1000);

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

            // are we in top color box area ?
            if (ypos < BOXSIZE) {               //draw white border on selected color box
                oldcolor = currentcolor;

                if (xpos < BOXSIZE) {
                    currentcolor = RED;
                    tft.drawRect(0, 0, BOXSIZE, BOXSIZE, WHITE);
                } else if (xpos < BOXSIZE * 2) {
                    currentcolor = YELLOW;
                    tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, WHITE);
                } else if (xpos < BOXSIZE * 3) {
                    currentcolor = GREEN;
                    tft.drawRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, WHITE);
                } else if (xpos < BOXSIZE * 4) {
                    currentcolor = CYAN;
                    tft.drawRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, WHITE);
                } else if (xpos < BOXSIZE * 5) {
                    currentcolor = BLUE;
                    tft.drawRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, WHITE);
                } else if (xpos < BOXSIZE * 6) {
                    currentcolor = MAGENTA;
                    tft.drawRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, WHITE);
                }

                if (oldcolor != currentcolor) { //rub out the previous white border
                    if (oldcolor == RED) tft.fillRect(0, 0, BOXSIZE, BOXSIZE, RED);
                    if (oldcolor == YELLOW) tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, YELLOW);
                    if (oldcolor == GREEN) tft.fillRect(BOXSIZE * 2, 0, BOXSIZE, BOXSIZE, GREEN);
                    if (oldcolor == CYAN) tft.fillRect(BOXSIZE * 3, 0, BOXSIZE, BOXSIZE, CYAN);
                    if (oldcolor == BLUE) tft.fillRect(BOXSIZE * 4, 0, BOXSIZE, BOXSIZE, BLUE);
                    if (oldcolor == MAGENTA) tft.fillRect(BOXSIZE * 5, 0, BOXSIZE, BOXSIZE, MAGENTA);
                }
            }
            // are we in drawing area ?
            if (((ypos - PENRADIUS) > BOXSIZE) && ((ypos + PENRADIUS) < tft.height())) {
                tft.fillCircle(xpos, ypos, PENRADIUS, currentcolor);
            }
            // are we in erase area ?
            // Plain Touch panels use bottom 10 pixels e.g. > h - 10
            // Touch panels with icon area e.g. > h - 0
            if (ypos > tft.height() - 10 && xpos >tft.width()/2) {
                // press the bottom of the screen to erase
                tft.fillRect(0, BOXSIZE, tft.width(), tft.height() - BOXSIZE, BLACK);
            }
            if (ypos > tft.height() - 10 && xpos <=tft.width()/2) {
                return;
                
            }
        }
    }
}    






 void Test_Read_Encoder_function(void) {
   
    for(;;) {
        if (uint8_t a=Read_Encoder())
            Serial.println(a);

    }
 }   

 void Test_buttons(void) {
    for(;;) {
        Serial.print("Button1="); Serial.println(digitalRead(PIN_BUTTON1));
        Serial.print("Button2="); Serial.println(digitalRead(PIN_BUTTON2));
        Serial.print("Button3="); Serial.println(digitalRead(PIN_BUTTON3));
        Serial.print("Button4="); Serial.println(digitalRead(PIN_BUTTON4));
        Serial.println("");
        delay(500);

    }
 }



