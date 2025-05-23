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

void Setup_PWM(void) {

  // ---------------- PWML0 Set-up on PIN34 (C.2)
  PMC->PMC_PCER1 |= PMC_PCER1_PID36;                   /// PMC(Power Management Control) -> PMC(Peripheral Clock Enable Register 1) |= PID36(Peripheral Idenitifier 36 = PWM Controller)
  PWM->PWM_DIS = PWM_DIS_CHID0;                        // Disable PWM channel 0

//Configure the pin C.2  as PWML0 Set-up (PIN34)
  PIOC->PIO_PDR |= PIO_PDR_P2;                          // PIOC(Parallel Input/Output Controller PORTC) -> PIO_PDR(PIO DIsable register) |=  PIO_PDR_P2 (C.2) (Pg.634) (Função do pino vai passar a ser PWM)
  PIOC->PIO_ABSR |= PIO_PC2B_PWML0;                    // Set PWM pin perhipheral type 
  
  // Enable the PWM channel 0 (see datasheet page 973)
  PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);    // Set the PWM clock rate to 2MHz (84MHz/42). Adjust DIVA for the resolution you are looking for
  PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_CLKA;       // The period is left aligned, clock source as CLKA on channel 2
  PWM->PWM_CH_NUM[0].PWM_CPRD = 100;                    // Channel 0 : Set the PWM frequency 2MHz/(2 * CPRD) = F ;
  Set_PWM_channel(PWM_RESISTOR_0);
  PWM->PWM_ENA = PWM_ENA_CHID0;                         // ENable PWM Channel 0


  // ---------------- PWML1 Set-up on PIN36 (C.4)
  PMC->PMC_PCER1 |= PMC_PCER1_PID36;                    // PMC(Power Management Control) -> PMC(Peripheral Clock Enable Register 1) |= PID36(Peripheral Idenitifier 36 = PWM Controller)
  PWM->PWM_DIS = PWM_DIS_CHID1;                         // Disable PWM channel 1

//Configure the pin C.4  as PWML1 Set-up (PIN36)
  PIOC->PIO_PDR |= PIO_PDR_P4;                          // PIOC (Parallel Input/Output Controller PORTC) -> PIO_PDR(PIO DIsable register) |=  PIO_PDR_P4 (C.4) (Pg.634) (Função do pino vai passar a ser PWM)
  PIOC->PIO_ABSR |= PIO_PC4B_PWML1;                     // Set PWM pin perhipheral type 
  
  // Enable the PWM channel 1 (see datasheet page 973)
  PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);    // Set the PWM clock rate to 2MHz (84MHz/42). Adjust DIVA for the resolution you are looking for
  PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA;       // The period is left aligned, clock source as CLKA on channel 2
  PWM->PWM_CH_NUM[1].PWM_CPRD = 100;                    // Channel 1 : Set the PWM frequency 2MHz/(2 * CPRD) = F ;
  Set_PWM_channel(PWM_RESISTOR_1);
  PWM->PWM_ENA = PWM_ENA_CHID1;                         // ENable PWM Channel 1


  // ---------------- PWML2 Set-up on PIN38 (C.6)
  PMC->PMC_PCER1 |= PMC_PCER1_PID36;                    // PMC(Power Management Control) -> PMC(Peripheral Clock Enable Register 1) |= PID36(Peripheral Idenitifier 36 = PWM Controller)
  PWM->PWM_DIS = PWM_DIS_CHID2;                         // Disable PWM channel 2

//Configure the pin C.6  as PWML2 Set-up (PIN38)
  PIOC->PIO_PDR |= PIO_PDR_P6;                          // PIOC (Parallel Input/Output Controller PORTC) -> PIO_PDR(PIO DIsable register) |=  PIO_PDR_P6 (C.6) (Pg.634) (Função do pino vai passar a ser PWM)
  PIOC->PIO_ABSR |= PIO_PC6B_PWML2;                     // Set PWM pin perhipheral type 
  
  // Enable the PWM channel 2 (see datasheet page 973)
  PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);    // Set the PWM clock rate to 2MHz (84MHz/42). Adjust DIVA for the resolution you are looking for
  PWM->PWM_CH_NUM[2].PWM_CMR = PWM_CMR_CPRE_CLKA;       // The period is left aligned, clock source as CLKA on channel 2
  PWM->PWM_CH_NUM[2].PWM_CPRD = 100;                    // Channel 2 : Set the PWM frequency 2MHz/(2 * CPRD) = F ;
  Set_PWM_channel(PWM_RESISTOR_2);
  PWM->PWM_ENA = PWM_ENA_CHID2;                         // ENable PWM Channel 2


  // ---------------- PWML3 Set-up on PIN40 (C.8)
  PMC->PMC_PCER1 |= PMC_PCER1_PID36;                    // PMC(Power Management Control) -> PMC(Peripheral Clock Enable Register 1) |= PID36(Peripheral Idenitifier 36 = PWM Controller)
  PWM->PWM_DIS = PWM_DIS_CHID3;                         // Disable PWM channel 1

  //Configure the pin C.8  as PWML3 Set-up (PIN40)
  PIOC->PIO_PDR |= PIO_PDR_P8;                          // PIOC (Parallel Input/Output Controller PORTC) -> PIO_PDR(PIO DIsable register) |=  PIO_PDR_P8 (C.8) (Pg.634) (Função do pino vai passar a ser PWM)
  PIOC->PIO_ABSR |= PIO_PC8B_PWML3;                     // Set PWM pin perhipheral type 
  
  // Enable the PWM channel 3 (see datasheet page 973)
  PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);    // Set the PWM clock rate to 2MHz (84MHz/42). Adjust DIVA for the resolution you are looking for
  PWM->PWM_CH_NUM[3].PWM_CMR = PWM_CMR_CPRE_CLKA;       // The period is left aligned, clock source as CLKA on channel 2
  PWM->PWM_CH_NUM[3].PWM_CPRD = 100;                    // Channel 3 : Set the PWM frequency 2MHz/(2 * CPRD) = F ;
  Set_PWM_channel(PWM_RESISTOR_3);
  PWM->PWM_ENA = PWM_ENA_CHID3;                         // ENable PWM Channel 3


  // ---------------- PWMH5 Set-up on PIN44 (C.19)
  PMC->PMC_PCER1 |= PMC_PCER1_PID36;                    // PMC(Power Management Control) -> PMC(Peripheral Clock Enable Register 1) |= PID36(Peripheral Idenitifier 36 = PWM Controller)
  PWM->PWM_DIS = PWM_DIS_CHID5;                         // Disable PWM channel 5

  //Configure the pin C.19  as PWMH5 Set-up (PIN44)
  PIOC->PIO_PDR |= PIO_PDR_P19;                          // PIOC (Parallel Input/Output Controller PORTC) -> PIO_PDR(PIO DIsable register) |=  PIO_PDR_P8 (C.8) (Pg.634) (Função do pino vai passar a ser PWM)
  PIOC->PIO_ABSR |= PIO_PC19B_PWMH5;                     // Set PWM pin perhipheral type 
  
  // Enable the PWM channel 5 (see datasheet page 973)
  PWM->PWM_CLK = PWM_CLK_PREA(0) | PWM_CLK_DIVA(42);    // Set the PWM clock rate to 2MHz (84MHz/42). Adjust DIVA for the resolution you are looking for
  PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPRE_CLKA;       // The period is left aligned, clock source as CLKA on channel 2
  PWM->PWM_CH_NUM[5].PWM_CPRD = 100;                    // Channel 5 : Set the PWM frequency 2MHz/(2 * CPRD) = F ;
  Set_PWM_channel(PWM_BUZZER);
  PWM->PWM_ENA = PWM_ENA_CHID5;

}


void Set_PWM_channel(int channel) {
  
    switch(channel) {
        case PWM_RESISTOR_0:    PWM->PWM_CH_NUM[0].PWM_CDTY = pwm_driver[PWM_RESISTOR_0];    // Channel 0: Set the PWM duty cycle to x%= (CDTY/ CPRD)  * 100 % ;
                                break;
        case PWM_RESISTOR_1:    PWM->PWM_CH_NUM[1].PWM_CDTY = pwm_driver[PWM_RESISTOR_1];  // Channel 1: Set the PWM duty cycle to x%= (CDTY/ CPRD)  * 100 % ;
                                break;
        case PWM_RESISTOR_2:    PWM->PWM_CH_NUM[2].PWM_CDTY = pwm_driver[PWM_RESISTOR_2];   // Channel 2: Set the PWM duty cycle to x%= (CDTY/ CPRD)  * 100 % ;
                                break;
        case PWM_RESISTOR_3:    PWM->PWM_CH_NUM[3].PWM_CDTY =pwm_driver[PWM_RESISTOR_3];  // Channel 3: Set the PWM duty cycle to x%= (CDTY/ CPRD)  * 100 % ;
                                break;
        case PWM_BUZZER:        PWM->PWM_CH_NUM[5].PWM_CDTY = pwm_driver[PWM_BUZZER];   // Channel 5: Set the PWM duty cycle to x%= (CDTY/ CPRD)  * 100 % ;
                                break;
    }
}

