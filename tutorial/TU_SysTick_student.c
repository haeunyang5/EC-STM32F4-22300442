/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Haeun Yang
Created          : 05-03-2021
Modified         : 09-028-2026
Language/ver     : C++ in VS Code

Description      : Tutorial - count up numbers at a constant rate using SysTick
/----------------------------------------------------------------*/


#include "stm32f411xe.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecSysTick2.h"  

#define LED_JK PB_12  // LED for Eval Board	JKIT

void setup(void)
{
    RCC_PLL_init();                 // System Clock = 84MHz	
    SysTick_init(); 
    GPIO_init(LED_JK, OUTPUT);       // LED for Eval Board	JKIT    
}


int main(void) {	    
	setup();
	while(1){
        GPIO_write(LED_JK, HIGH);        
        delay_ms (1000);
        GPIO_write(LED_JK, LOW);        
        delay_ms (1000);        
	}
}
