/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Haeun Yang
Student ID       : 22300442
Created          : 10-22-2026 
Language/ver     : C in VS Code

Description      : EC Test 1 (2026) Programming Test
                   
                   Rename this file to  EC_Test1_yourID.c
/----------------------------------------------------------------*/
#include "stm32f411xe.h"

//==================================================================
//  EC Library Include
//==================================================================
#include "ecSTM32F4v2.h"

//==================================================================
// User GPIO Pin definitions
//==================================================================
#define SW1  PD_2
#define SW2  PA_4
#define PHOTO_SENSOR PA_0
#define LED  PB_12


// Stepper ( A, B, AN, BN )
PinName_t pinStepper[4]    = {PC_5,PC_6,PC_7,PC_8} ;

// FND segments
PinName_t _pinFND[8]    = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7} ;

// FND digit select : FND_0 ~ FND_3
PinName_t _selectFND[4] = {PA_10, PA_11, PC_4, PC_3} ;

// Global Variables
// use   volatile uint32_t   instead of uint32_t if used in Interrupt ISR
volatile uint32_t motor_mode   = 0;	
volatile uint32_t motor_dir    = 0;	 // 0: CW, 1: CCW
volatile uint32_t motor_speed  = 0;	
static volatile uint32_t state = 0 ;
static uint8_t PC_Data = 0;


//==================================================================
//  Setup 
//==================================================================

void pinFNDsetup(PinName_t *pinFND){
    for (int i=0; i<8; i++){
        // GPIO_otype(_pinFND[i], PUSH_PULL) ;  
        GPIO_init(pinFND[i], OUTPUT) ;   
        GPIO_pupd(pinFND[i], NO_PUPD) ;
        GPIO_ospeed(pinFND[i], MEDIUM_SPEED) ;
    }
}

void selectFNDsetup(PinName_t *selectFND){
    for (int i=0; i<4; i++){
        GPIO_init(selectFND[i], OUTPUT) ;  
        GPIO_otype(selectFND[i], PUSH_PULL) ;
    }
}

//State number 
typedef enum {
	Mode0, Mode1, Mode2, Mode3
} StateMode;

static const StateStepper_t fsm_Mode[4] = {	
	// state, motor speed, FND value
	{{Mode1,Mode0},{0,0}},	
 	{{Mode2,Mode0},{2,1}},	
 	{{Mode3,Mode1},{6,2}},	
 	{{Mode3,Mode2},{10,3}}
};

void setup(void){
	RCC_PLL_init();					// System Clock = 84MHz
	SysTick_init();					// SysTick, 1 msec
	
	// USART2, 9600 baud
	USART2_init(); 		

	// GPIO Inputs
    GPIO_init(SW1, INPUT) ;  
    GPIO_pupd(SW1, PU) ;

    GPIO_init(SW2, INPUT) ; 
    GPIO_pupd(SW2, PU) ;

	// EXTI 
    EXTI_init(SW2, FALL, 10);
    EXTI_init(SW1, FALL, 10);
	
	// GPIO Outputs	
    GPIO_init(LED, OUTPUT) ;  

	// Stepper
	stepper_init(FULL, pinStepper); 	         
	stepper_speed(0) ;

	// Other Setup
    FND_init(_pinFND);
    FND_select_init(_selectFND) ;    
    selectFNDsetup(_selectFND) ;
    pinFNDsetup(_pinFND) ;   
}


//==================================================================
//  Main
//==================================================================
int main(void){
	setup();
	printf("MCU Initialized\r\n") ;
	int buttonState = 0 ;
	int numDisplay[4]  = {0, 1, 2 ,3} ;
    uint8_t selectFND1  = 0 ;
    uint8_t selectFND2  = 1 ;
	uint32_t rev=1 ;
	uint32_t steps=stepper_stepPerRev()*rev ;

	while(1){

		// Related to controlling motor speed/dir etc
		// Display choice: FND1 for CW, FND2 for CCW
		if (motor_dir == 0) { // CW
			FND_select(selectFND1);
		}
		else { // CCW
			FND_select(selectFND2);
		}
		FND_display(_pinFND, numDisplay[fsm_Mode[state].out[1]]) ;

		// HINT:  use  stepper_speed(vel);	only when vel>0
		if (motor_speed > 0)  stepper_step(1, motor_dir);
		stepper_step(10, motor_dir);	 // Keep step=1 or a low value to stop blocking other code
		// stepper_speed(fsm_Mode[0].out[0]) ; 
 

		// Check PAUSE/RESUME 
		// Print to the serial monitor every 2 sec
		// PAUSE, blink at 2 sec
        buttonState = GPIO_read(PHOTO_SENSOR) ;
        if(buttonState == 1)  {
			GPIO_write(LED, HIGH) ; // 안 눌림
			delay_ms(2000) ;
			printf("Mode=%d " , fsm_Mode[state].out[1]) ;
			printf("Speed=%d (rpm) ", fsm_Mode[state].out[0]) ;
			printf("DIR=%d (0:CW, 1:CCW)\r\n", motor_dir) ;
		}
        else {
			GPIO_write(LED, HIGH) ; // 눌림
			delay_ms(2000) ;
			GPIO_write(LED, LOW) ;
			printf("Mode=%d " , fsm_Mode[state].out[1]) ;
			printf("Speed=%d (rpm) ", fsm_Mode[state].out[0]) ;
			printf("DIR=%d (0:CW, 1:CCW)\r\n", motor_dir) ;
		}
			
	}
}


//==================================================================
//  EXTI Interrupt Service Routines
//==================================================================
// SW1 is pressed -> increase the motor speed
void EXTI2_IRQHandler(void) {
	if (is_pending_EXTI(SW1)) {    
		state = fsm_Mode[state].next[0] ;
		motor_speed = fsm_Mode[0].out[0] ; 
		if (motor_speed > 0) stepper_speed(motor_speed);
		clear_pending_EXTI(SW1) ;
	}
}
// SW2 is pressed -> decrease motor speed
void EXTI4_IRQHandler(void) {
    if (is_pending_EXTI(SW2)) {   
        state = fsm_Mode[state].next[1];
        motor_speed = fsm_Mode[state].out[0];
        if (motor_speed > 0) stepper_speed(motor_speed);
		clear_pending_EXTI(SW2) ;
	}
}



//==================================================================
//  UART2 Interrupt Service Routines
//==================================================================
void USART2_IRQHandler(void){
	if(is_USART2_RXNE()){
		PC_Data = USART2_read();		// RX from UART2 (PC)
		USART2_write(&PC_Data,1);		// TX to USART2	 (PC)	 Echo of keyboard typing			
		if(PC_Data == 'C' || PC_Data == 'c') motor_dir = 0 ;
		if(PC_Data == 'B' || PC_Data == 'b') motor_dir = 1 ;
	}
}
