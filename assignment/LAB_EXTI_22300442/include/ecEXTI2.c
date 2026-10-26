/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : [ YOUR NAME GOES HERE !!!!!]
Created          : 05-03-2021
Modified         : 05-03-2026 [WRITE THE DATE!!!!]
Language/ver     : C++ in VS Code

Description      : [WRITE DESCRIPTION HERE!!!!]
/----------------------------------------------------------------*/


#include "ecGPIO2.h"
#include "ecSysTick2.h"
#include "ecEXTI2.h"


void EXTI_init(PinName_t pinName, uint32_t trig_type,uint32_t priority){

	GPIO_TypeDef  *Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);
	
	// Enable EXTI Clock with SYSCFG controller clock 	
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;		
	

	// Connect EXTI to the GPIO Port_y Pin_x
	// Pin 0~3: EXTICR1 // EXTICR[0]
	// Pin 4~7: EXTICR2 // EXTICR[1]
	// Pin 8~11: EXTICR3 // EXTICR[2]
	// Pin 12~15: EXTICR4 // EXTICR[3]	
	uint32_t EXTICR_port=0;
	if		(Port == GPIOA) EXTICR_port = 0 ;
	else if	(Port == GPIOB) EXTICR_port = 1 ;
	else if	(Port == GPIOC) EXTICR_port = 2 ;
	else if	(Port == GPIOD) EXTICR_port = 3 ;
	else					EXTICR_port = 4 ;
	
	SYSCFG->EXTICR[pin / 4] &= ~(0xF<<((pin % 4) * 4)) ;		   // clear 4 bits
	SYSCFG->EXTICR[pin / 4] |= (EXTICR_port<<((pin % 4) * 4)) ;	   // Set 4 bits
	

	// Select  Trigger edge (RISE, FALL, BOTH)
	if (trig_type == FALL) 		EXTI->FTSR |= 1UL << pin ;  // Falling trigger enable 
	else if	(trig_type == RISE) EXTI->RTSR |= 1UL << pin ;   // Rising trigger enable 
	else if	(trig_type == BOTH) {			// Both falling/rising trigger enable
		EXTI->RTSR |= 1UL << pin ; 
		EXTI->FTSR |= 1UL << pin ;
	}
	// Enable EXTI, with Not-Masked Interrupt Request 
	// Not-mask (==Enable) EXTIx
 	EXTI->IMR |= 1UL << pin;
	
	
	// NVIC(IRQ) Setting
	uint32_t EXTI_IRQn = 0;
	if (pin < 5) 	    EXTI_IRQn = (6 + pin);
	else if	(pin < 10) 	EXTI_IRQn = EXTI9_5_IRQn;
	else 				EXTI_IRQn = EXTI15_10_IRQn;
		
	NVIC_SetPriority(EXTI_IRQn, priority);	
	NVIC_EnableIRQ(EXTI_IRQn); 					
}


void EXTI_enable(PinName_t pinName) {
	GPIO_TypeDef  *Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);
	
	EXTI->IMR |= 1 << pin;     // not masked (i.e., Interrupt enabled)
}

void EXTI_disable(PinName_t pinName) {
	GPIO_TypeDef  *Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);

	EXTI->IMR |= 0 << pin;      // masked (i.e., Interrupt disabled)
}



uint32_t is_pending_EXTI(PinName_t pinName) {
	GPIO_TypeDef  *Port;
	unsigned int pin;
	ecPinmap(pinName,&Port,&pin);
    
	uint32_t EXTI_PRx = (EXTI->PR>>pin & 1);     	// read PRx, EXTI pending bit.
	return EXTI_PRx;  								// Return only 0 or 1
}


void clear_pending_EXTI(PinName_t pinName){
	// Port, Pin Configuration
	GPIO_TypeDef *Port;
	unsigned int pin;
	ecPinmap(pinName, &Port, &pin);

	// clear by writing 1 to the pending bit
	EXTI->PR  |= (1 << pin);     
}
