#include <Arduino.h>
#include <SPI.h>          // f.k. for Arduino-1.5.2
#include "Adafruit_GFX.h"// Hardware-specific library
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <Adafruit_MAX31865.h>




// All the mcufriend.com UNO shields have the same pinout.
// i.e. control pins A0-A4.  Data D2-D9.  microSD D10-D13.
// Touchscreens are normally A1, A2, D7, D6 but the order varies
//
// This demo should work with most Adafruit TFT libraries
// If you are not using a shield,  use a full Adafruit constructor()
// e.g. Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

#define PIN_ENCODER_INPUT_A 24
#define PIN_ENCODER_INPUT_B 27
#define PIN_ENCODER_INPUT_PUSH 26

#define PIN_RELAY1_OUTPUT 23
#define PIN_RELAY2_OUTPUT 22
#define PIN_RELAY3_OUTPUT 25

#define PIN_BUTTON1 30
#define PIN_BUTTON2 31
#define PIN_BUTTON3 28
#define PIN_BUTTON4 29


#define PIN_SPI_CS1 52
#define PIN_SPI_CS2 53
#define PIN_SPI_DI  51
#define PIN_SPI_DO  50
#define PIN_SPI_CLK 49





#define MINPRESSURE 100
#define MAXPRESSURE 1000
#define N_POINTS_TEMPERATURE 6
#define DX 4
#define DY 4

// Assign human-readable names to some common 16-bit color values:
#define BAR_MENU_COLOR tft.color565(0,122,204)
#define MENU_COLOR_BACKGROUND_DEFAULT tft.color565(0,0,128)
#define MENU_COLOR_LETTERS_DEFAULT tft.color565(120,120,120)
#define MENU_COLOR_BACKGROUND_SELECTED tft.color565(0,0,200)
#define MENU_COLOR_LETTERS_SELECTED tft.color565(255,255,255)
#define MENU_COLOR_BACKGROUND_ACTIVACTED tft.color565(255,0,0)
#define MENU_COLOR_LETTERS_ACTIVACTED tft.color565(255,255,255)

#define GRAPH_WATEMARK_COLOR tft.color565(40,40,40)
#define MAIN_MENU_ACTIVE 
#define MAIN_MENU_LETTERS tft.color565(200,200,200)

#define	BLACK   0x0000
#define GRAY    0x2222
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define TRUE 1
#define FALSE 0

#define N_MAX_GRAPHS 4
#define N_POINTS_TIME_AXIS 300

//Valores para representar os estados lidos do encoder rotativo (!!! Cada um, um bit !!!)
#define STEADY 0
#define LEFT   1
#define RIGHT  2
#define ERROR  4
#define PUSH   8

// The value of the Rref resistor. Use 430.0 for PT100 and 4300.0 for PT1000
#define RREF      430.0
// The 'nominal' 0-degrees-C resistance of the sensor
// 100.0 for PT100, 1000.0 for PT1000
#define RNOMINAL  100.0

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef _swap_int16_t
#define _swap_int16_t(a, b)                                                    \
  {                                                                            \
    int16_t t = a;                                                             \
    a = b;                                                                     \
    b = t;                                                                     \
  }
#endif



typedef struct {
    int  time;
    int  temp;   
} STRUCT_GRAPH_POINTS;
 


enum {
    PWM_RESISTOR_0,
    PWM_RESISTOR_1,
    PWM_RESISTOR_2,
    PWM_RESISTOR_3,
    PWM_BUZZER
};

enum {
    ICON_NULL,
    ICON_HOME,
    ICON_LEFT_ARROW,
    ICON_RIGHT_ARROW,
    ICON_NEXT,
    ICON_OK,
    ICON_CANCEL
}; 







unsigned long testFillScreen();
unsigned long testText();
unsigned long testLines(uint16_t color);
unsigned long testFastLines(uint16_t color1, uint16_t color2);
unsigned long testRects(uint16_t color);
unsigned long testFilledRects(uint16_t color1, uint16_t color2);
unsigned long testFilledCircles(uint8_t radius, uint16_t color);
unsigned long testCircles(uint8_t radius, uint16_t color);
unsigned long testTriangles();
unsigned long testFilledTriangles();
unsigned long testRoundRects();
unsigned long testFilledRoundRects();
void progmemPrint(const char *str);
void progmemPrintln(const char *str);
void showhanzi(unsigned int x, unsigned int y, unsigned char index);
void runtests(void);

//main.c
void setup(void);
void loop(void);
//graphic_functions.cpp
void my_drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void writeLine2(int16_t x0, int16_t y0, int16_t x1, int16_t y1,uint16_t color);
void draw_icon(int x, uint8_t n_icon);
void draw_top_bar_menu(void);
void Draw_graph(uint8_t n_graph);
//pwmc.cpp
void Setup_PWM(void);
void Set_PWM_channel(int channel);
//Testes.cpp
void Test_PWM(void);
void Test_hand_drawing(void);
void Test_Encoder(void);
void Test_Read_Encoder_function(void);
void Test_Outputs(void);
void Test_buttons(void);
//graphs.cpp
void Edit_graph(void);
void  Test_graph(void);
//menus.cpp
void Main_menu(void);
void Draw_bar_menus(bool clean_screen, int n_option_selected, int n_option_activacted);
//owen.cpp
void Main_owen_function(void);
void Setup_MAX31865(void);

void Update_array_temperatures(int n_graph);
void Fill_temp_points(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
//Encoder.cpp
void Setup_Encoder(void);
uint8_t Read_Encoder(void);
//Snake.cpp
void snake_game(void);
//MAX38165.cpp
void Test_MAX31865(void);
uint8_t Read_temperature_owen(void);
uint8_t Read_temperature_intera(void);



