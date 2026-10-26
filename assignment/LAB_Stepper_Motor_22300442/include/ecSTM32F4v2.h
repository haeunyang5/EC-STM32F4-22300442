//ecSTM32F4v2.h


/*
******************************************************************************
* @course   Embedded Controller- HGU
* @author	iiLAB
* @mod		2024-8-05 by YKKIM
* @brief	STM32F411 Library for EC
* Author           : Haeun Yang
* Language/ver     : C++ in VS Code
******************************************************************************
*/

#ifndef __EC_STM_H
#define __EC_STM_H

// STM built-In Library
#include "stm32f4xx.h"
#include "stm32f411xe.h"
#include "math.h"

// EC course Library
#include "ecPinNames.h"
#include "ecRCC2.h"
#include "ecGPIO2.h"
#include "ecEXTI2.h"
#include "ecSysTick2.h"
//#include "ecTIM2.h"
//#include "ecPWM2.h"
//#include "ecStepper2.h"
//#include "ecADC2.h"
//#include "ecUART2.h"



void MCU_init(void){
    // CLOCK PLL 84MHz
    RCC_PLL_init();
    
    // SysTick 1msec for delay
    SysTick_init();    
    
    // Button PC13
    GPIO_init(PC_13, INPUT);
    GPIO_pupd(PC_13, PD);    
    
    // LED PA5
    GPIO_init(PA_5, OUTPUT);    

    // TIMx Default Initialization
    // ...
    // PWM Default Initialization
    // ...        
    // USART Default Initialization
    // ...
    // Others
}

#endif



 