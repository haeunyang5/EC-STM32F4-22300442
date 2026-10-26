#include "ecSTM32F4v2.h"

static volatile uint8_t PC_Data = 0;
static volatile uint8_t BT_Data = 0;
uint8_t PC_string[]="Loop:\r\n";

#define LD2   PA_5

void setup(void){
	RCC_PLL_init();
	SysTick_init();
	
	// USART2: USB serial init
	USART2_init();  // default 9600
	//USART2_baud(BAUD_38400);
	
	// USART1: MCU2 serial init 
	USART1_init(); // default 9600
	//USART1_baud(BAUD_38400);
}

int main(void){	
	setup();
	LEDsetup(LD2) ;
	printf("MCU Initialized\r\n");	
	while(1){
		// USART Receive: Use Interrupt only
		// USART Transmit: Polling
		// USART2_write(PC_string, 7);
        if (BT_Data == 'L') GPIO_write(LD2, LOW) ; // Press key "L" to Turn OFF LED
        if (BT_Data == 'H') GPIO_write(LD2, HIGH) ; // Press key "H" to Turn ON LED
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
	}
}

void LEDsetup(uint32_t  LED){
    GPIO_init(LED, OUTPUT) ;   // Initialze the LED pin as an output
    GPIO_pupd(LED, PU) ;
    GPIO_otype(LED, PUSH_PULL) ;
    GPIO_ospeed(LED, MEDIUM_SPEED) ;
}