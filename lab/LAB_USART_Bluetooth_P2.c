/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Haeun Yang
Created          : 05-03-2021
Modified         : 10-01-2026
Language/ver     : C++ in VS Code

Description      :LAB BLUETOOTH USING UART
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

static volatile uint8_t PC_Data = 0;
static volatile uint8_t BT_Data = 0;
static volatile uint8_t cmd     = 0;     // motor command (J, K)

#define LED_PIN   PA_5
PinName_t pinStepper[4]    = {PB_10,PB_4,PB_5,PB_3};

void setup(void){
	RCC_PLL_init();
	SysTick_init();

	USART2_init();  // default 9600
	USART1_init(); // default 9600

	stepper_init(FULL, pinStepper);  // Stepper GPIO pin initialization
	stepper_speed(10);                //  set stepper motor speed
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
			cmd = 0 ;             // clear command 
			stepper_step(steps, DIR_CW) ;
        }
        if (BT_Data == 'K') {
			cmd = 0 ;
			stepper_step(steps, DIR_CCW) ;
        }
	}
}

void USART2_IRQHandler(){               // USART2 RX Interrupt 
	if(is_USART2_RXNE()){
		PC_Data = USART2_read();		// RX from UART2 (PC)
		USART2_write(&PC_Data,1);		// TX to USART2	 (PC)	 Echo of keyboard typing
	}
}


void USART1_IRQHandler(){                    // USART1 RX Interrupt (Bluetooth)
	if (is_USART1_RXNE()){
		BT_Data = USART1_read();             // RX from Bluetooth
		printf("RX: %c \r\n", BT_Data);      // display on PC (USART2)

		if      (BT_Data == 'L') GPIO_write(LED_PIN, LOW);
		else if (BT_Data == 'H') GPIO_write(LED_PIN, HIGH);
		else if (BT_Data == 'J' || BT_Data == 'K') cmd = BT_Data;
		else if (BT_Data == 'P'){
			cmd = 0;
			stepper_stop();                  // emergency stop
		}
	}
}