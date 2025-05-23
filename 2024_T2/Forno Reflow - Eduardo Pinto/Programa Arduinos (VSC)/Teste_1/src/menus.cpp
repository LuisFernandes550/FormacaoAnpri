#include"my_defs.h"

extern int XP,XM,YP,YM; // IOs para o touch
extern int TS_LEFT,TS_RT,TS_TOP,TS_BOT; 
extern MCUFRIEND_kbv tft;
extern TouchScreen ts; 
extern TSPoint tp;
extern uint8_t Orientation;

extern const char *button_menus_strings[5];

void Draw_bar_menus(bool clean_screen, int n_option_selected, int n_option_activacted) {
int i;
int color_background,color_letters;

    if (clean_screen)
        tft.fillScreen(BLACK);


    tft.setTextSize(3);
    
    for(i=0;i<5;i++) {
        if (strlen(button_menus_strings[i])) {
            color_background=MENU_COLOR_BACKGROUND_DEFAULT;
            color_letters=MENU_COLOR_LETTERS_DEFAULT;
            

            if (n_option_selected==i) {
                color_background=MENU_COLOR_BACKGROUND_SELECTED;
                color_letters=MENU_COLOR_LETTERS_SELECTED;
            }        
            if (n_option_activacted==i) {
                color_background=MENU_COLOR_BACKGROUND_ACTIVACTED;
                color_letters=MENU_COLOR_LETTERS_ACTIVACTED;
            }                
            
            tft.fillRect(50, 50+i*80, 220, 50, color_background);    
            tft.setTextColor(color_letters, color_background);
        
            tft.drawRect(49,50+i*80,222,51,TFT_DARKGREY);
            tft.setCursor(50+(220/2)-strlen(button_menus_strings[i])*9, 65+i*80); 
            tft.print(button_menus_strings[i]);
            
        }
    
    }

}

void Main_menu(void) {
int i;
uint16_t xpos=5000, ypos=5000;  //screen coordinates (ypos=5000 para n detectar top bar menu)
int n_option_menu_active=0; //-1 significa nenhum activo, 0=primeiro opção menu, 2=Segunda...
int n_options_menu;

    n_options_menu=5;
    button_menus_strings[0]="OWEN";
    button_menus_strings[1]="PROFILES";
    button_menus_strings[2]="TEST PWM";
    button_menus_strings[3]="TEST OUT";
    button_menus_strings[4]="DRAW";

    Draw_bar_menus(TRUE, n_option_menu_active,-1);
    for(;;) {

        uint8_t enc=Read_Encoder();
        
        if (enc&PUSH) {
            Draw_bar_menus(FALSE, -1 ,n_option_menu_active); 
            delay(200);
            switch(n_option_menu_active) {
                case 0: Main_owen_function();
                        return;
                case 1: Edit_graph();
                        return;
                case 2: Test_PWM();
                        return;
                case 3: Test_Outputs();
                        return;        
                case 4: //snake_game();
                        Test_hand_drawing();
                        return;

            }  
        }
        else {
        
            switch(enc) {
                case STEADY:break;
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
                tft.setTextColor(MENU_COLOR_LETTERS_ACTIVACTED, MENU_COLOR_BACKGROUND_ACTIVACTED);
                tft.setTextSize(3);
    
                tft.fillRect(50, 50+i*80, 220, 50, MENU_COLOR_BACKGROUND_ACTIVACTED);
                tft.setCursor(50+(220/2)-strlen(button_menus_strings[i])*9, 65+i*80); 
                tft.print(button_menus_strings[i]);
                delay(200);
                switch(i) {
                case 0: Main_owen_function();
                        return;
                case 1: Edit_graph();
                        return;
                case 2: Test_PWM();
                        return;
                case 3: Test_Outputs();
                        return;        
                case 4: //snake_game();
                        Test_hand_drawing();
                        return;
                }
            }
        }                   
    }

}