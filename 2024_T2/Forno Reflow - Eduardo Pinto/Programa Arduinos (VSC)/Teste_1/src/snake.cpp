#include "my_defs.h"

//1=main menu,2=game,3=game over
byte screen=1;

//Size of the snake (on proper situation it would be 22x23=506 positions of the grid, though it's limited by arduino's memory
#define snakesize 250
//Snake's speed between movement(miliseconds)
#define velocity 200

#define maxx 230
#define minx 190
#define maxy 80
#define miny 40
#define gridx 24
#define gridy 23

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

void printScore(int score){
  tft.fillRect(90,297,90,20,BLACK);
  tft.setCursor(90, 297);
  tft.print(score);
}

void updateSnake(byte newPos[], byte oldPos[]){
  tft.fillRect(oldPos[0]*10,oldPos[1]*10,10,10,BLACK);
  tft.fillRect(newPos[0]*10,newPos[1]*10,10,10,WHITE);
}


void printGameOver(int score, byte grid[gridy][gridx]){
  screen=3;
  for(int y=0;y<gridy;y++){
    for(int x=0;x<gridx;x++){
      grid[y][x]=0;
    }
  }
  tft.fillRect(40,55,160,180,TFT_LIGHTGREY);
  tft.drawRect(41,56,158,178,TFT_MAROON);
  tft.setTextColor(BLACK);
  tft.setTextSize(4);
  tft.setCursor(55, 60);
  tft.print("Game");
  tft.setCursor(90, 90);
  tft.print("Over");
  tft.setTextColor(TFT_NAVY);
  tft.setTextSize(3);
  tft.setCursor(70, 120);
  tft.print("Score");
  tft.drawRect(65,150,110,30,TFT_NAVY);
  tft.setTextColor(TFT_DARKCYAN);
  tft.setCursor(70, 155);
  tft.print(score);
  tft.fillRect(80,185,80,30,TFT_RED);
  tft.setCursor(85, 188);
  tft.setTextColor(WHITE);
  tft.print("Play");
}

void printGameScreen(){
  screen=2;
  tft.fillScreen(BLACK);
  tft.drawLine(0, 230, 240, 230, TFT_YELLOW);
  //Arrow up-1
  tft.drawLine(30,240,30,280,WHITE);
  tft.drawLine(30,240,10,260,WHITE);
  tft.drawLine(30,240,50,260,WHITE);
  //Arrow right-2
  tft.drawLine(70,260,110,260,WHITE);
  tft.drawLine(110,260,90,240,WHITE);
  tft.drawLine(110,260,90,280,WHITE);
  //Arrow down-3
  tft.drawLine(150,240,150,280,WHITE);
  tft.drawLine(150,280,130,260,WHITE);
  tft.drawLine(150,280,170,260,WHITE);
  //Arrow left-4
  tft.drawLine(190,260,230,260,WHITE);
  tft.drawLine(190,260,210,240,WHITE);
  tft.drawLine(190,260,210,280,WHITE);
  //----------------
  tft.drawLine(0, 290, 240, 290, TFT_YELLOW);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(10 , 297);
  tft.println("Score: ");
  printScore(0);
  tft.fillRect(180,295,50,20,TFT_RED);
  tft.setCursor(182, 297);
  tft.print("Menu");
}

void printMenu(){
  screen=1;
  tft.fillScreen(BLACK);
  tft.setTextSize(5);
  tft.setTextColor(WHITE);
  tft.setCursor(30 , 30);
  tft.println("SNAKE!");
  tft.fillRect(50,120,120,50,TFT_GREEN);
  tft.setTextSize(3);
  tft.setCursor(63 , 133);
  tft.println("Start");
  tft.setCursor(0 , 310);
  tft.setTextSize(1);
  tft.println("Created by Arnau/Redrednose/mas886");
}

void snake_game(void) {
int xpos,ypos;
byte snakeBuffer[snakesize][2]={0};
byte grid[gridy][gridx]={0};
long addpos;
long delpos;
byte arrow, lastarrow;
int score;
byte newPos[2];
byte oldPos[2];
bool gendot;
bool incaxys;
byte increment;
bool initgame=false;

    while(true){
        //This if will update the snake position while we are in the game screen
        if (screen==2){
            if ((arrow!=0)&&(((lastarrow+2!=arrow)&&(lastarrow-2!=arrow))||(score==0))) {
                if((arrow%2)!=0){
                    incaxys=true;
                } else {
                    incaxys=false;
                }
                if((arrow>1)&&(arrow<4)){
                    increment=1;
                } else {
                    increment=-1;
                }
                lastarrow=arrow;
            }

            arrow=0;
            newPos[incaxys]+=increment;
            if ((newPos[0]<0)|(newPos[0]>23)|(newPos[1]<0)|(newPos[1]>22)) {        
                printGameOver(score,grid);
  
            }
            else {
                snakeBuffer[addpos][0]=newPos[0];
                snakeBuffer[addpos][1]=newPos[1];
                addpos++;
                if(addpos>(snakesize-1)){
                    addpos=0;
                }
                grid[newPos[1]][newPos[0]]++;
                grid[oldPos[1]][oldPos[0]]=0;

            //Check various grid events
            switch(grid[newPos[1]][newPos[0]]) {
                case 2: printGameOver(score, grid);
                        break;
                case 4: score+=10;
                        printScore(score);
                        gendot=true;
                        if((score/10)<snakesize){
                            delpos--;
                        }
                default: updateSnake(newPos,oldPos);
                        delpos++;
                        if (delpos>(snakesize-1)) {
                            delpos=0;
                        }
                        oldPos[0]=snakeBuffer[delpos][0];
                        oldPos[1]=snakeBuffer[delpos][1];
                        delay(velocity);
                        break;
                }
            }    
        }
        //We generate a random dot to feed the snake
        while(gendot) {
            byte posy=random(gridy-1);
            byte posx=random(gridx-1);
            if (grid[posy][posx]==0) {
                tft.fillRect(posx*10,posy*10,10,10,TFT_CYAN);
                grid[posy][posx]=3;
                gendot=false;
            }
        }
    
        // Button control, only triggered when there's data from the touchscreen
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

            switch(screen) {
                case 1: //menu and start game parameters
                        if ((xpos>=80)&&(xpos<=190)&&(ypos>=140)&&(ypos<=190)) {
                            initgame=true;
                        }
                        break;
                case 2: //Check arrows on in game screen
                        if (!((xpos>=10)&&(xpos<=60)&&(ypos>=10)&&(ypos<=25))){
                            for(int c=0,g=1;c<=180;c+=60,g++) {
                                if ((xpos>=minx-c)&&(xpos<=maxx-c)&&(ypos>=miny)&&(ypos<=maxy)) {
                                    arrow=g;
                                }
                            }
                        } else { 
                            if ((xpos>=10)&&(xpos<=60)&&(ypos>=10)&&(ypos<=25)) {
                                printMenu();
                            }
                        }    
                        break;
                case 3: if ((xpos>=10)&&(xpos<=60)&&(ypos>=10)&&(ypos<=25)) {
                            printMenu();
                         } else {
                            if ((xpos>=80)&&(xpos<=160)&&(ypos>=100)&&(ypos<=130)) {
                                initgame=true;
                            }
                        }    
                        break;
            }

        
            if (initgame) {//Set variables for the game initialization
                initgame=false;
                arrow=2;
                lastarrow=1;
                score=0;
                addpos=1;
                delpos=0;
                incaxys=false;
                increment=1;
                gendot=true;
                newPos[0]=-1;newPos[1]=0;
                printGameScreen();
            }
        }   //  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) 
    
    } // while(true)
} // snake_game()   

