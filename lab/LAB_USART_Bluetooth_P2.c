/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Haeun Yang
Created          : 05-03-2021
Modified         : 10-01-2026
Language/ver     : C++ in VS Code

Description      :LAB BLUETOOTH USING UART
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"
#include "ecUART2.h" 

static volatile uint8_t PC_Data = 0;
static volatile uint8_t BT_Data = 0;
uint8_t PC_string[]="Loop:\r\n";

#define LED_PIN   PA_5
#define BUTTON_PIN PC_13 
PinName_t pinStepper[4]    = {PB_10,PB_4,PB_5,PB_3};

void setup(void){
	RCC_PLL_init();
	SysTick_init();

	USART2_init();  // default 9600
	USART1_init(); // default 9600

	stepper_init(FULL, pinStepper);  // Stepper GPIO pin initialization
	stepper_speed(1);                //  set stepper motor speed
}

int main(void) { 
	// Initialiization --------------------------------------------------------
	setup();
	printf("MCU Initialized\r\n");	
	uint32_t rev=2;
	uint32_t steps=stepper_stepPerRev()*rev;
	
	// Inifinite Loop ----------------------------------------------------------
	while(1){
        if (BT_Data == 'J') {
            stepper_step(steps, DIR_CW);
            delay_ms(2000);
        }
        if (BT_Data == 'K') {
            stepper_step(steps, DIR_CCW);
            delay_ms(2000);
        }
        if (BT_Data == 'P') {
            stepper_stop();
            clear_pending_EXTI(BUTTON_PIN); // cleared by writing '1'
        }
		GPIO_write(LED_PIN, HIGH) ;
		delay_ms(2000);  
	}
}

void USART2_IRQHandler(){               // USART2 RX Interrupt 
	if(is_USART2_RXNE()){
		PC_Data = USART2_read();		// RX from UART2 (PC)
		USART2_write(&PC_Data,1);		// TX to USART2	 (PC)	 Echo of keyboard typing
		USART1_write(&PC_Data,1);	    // TX to Other MCU
	}
}

void USART1_IRQHandler(){            	// USART2 RX Interrupt
	if(is_USART1_RXNE()){
		BT_Data = USART1_read();		// RX from other MCU UART1		
		printf("RX: %c \r\n",BT_Data); 	// TX to USART2  (PC)
		if (BT_Data == 'L') GPIO_write(LED_PIN, LOW) ;
        if (BT_Data == 'H') GPIO_write(LED_PIN, HIGH) ;
	}
}