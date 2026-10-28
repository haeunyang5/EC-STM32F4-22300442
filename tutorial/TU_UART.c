/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 10-08-2026
Modified         : 00-00-2026 [WRITE THE DATE!!!!]
Language/ver     : C in VS Code

Description      : Tutorial - USART2 with the STUDENT library (ecUART2_student)
/----------------------------------------------------------------*/
#include <stdio.h>
#include "stm32f4xx.h"
#include "ecRCC2.h"
#include "ecSysTick2.h"
#include "ecGPIO2.h"
#include "ecUART2.h"


uint8_t PC_data = 0;

// Initialiization 
void setup(void){	
	RCC_PLL_init();
	SysTick_init();
	// USART2: USB serial init
	USART2_init();
}

int main(void) {
	// Initialiization --------------------------------------------------------
	setup();
	printf("MCU Initialized\r\n");
	// Inifinite Loop ----------------------------------------------------------
	while(1){		
		delay_ms(2000);        
	}
}


// ECHO keyboard inputs
void USART2_IRQHandler(){        // USART2 RX Interrupt : Recommended
	if(is_USART2_RXNE()){
		PC_data = USART2_read();		// RX from UART2 (PC)
		USART2_write(&PC_data,1);		// TX to USART2	 (PC)	 Echo of keyboard typing		
	}
}