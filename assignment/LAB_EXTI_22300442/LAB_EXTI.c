/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Haeun Yang
Created          : 05-03-2021
Modified         : 10-01-2026
Language/ver     : C++ in VS Code

Description      :Counter on 7-segment display using EXTI
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

#define SW1  PD_2
#define SW2  PA_4
PinName_t _pinFND[8]    = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7} ;
PinName_t _selectFND[4] = {PA_10, PA_11, PC_4, PC_3} ;

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

// Initialize
void setup(void){
    RCC_PLL_init();       // System Clock = 84MHz
    SysTick_init();       // SysTick Timer Initialization
    
    GPIO_init(SW1, INPUT) ;  
    GPIO_pupd(SW1, PU) ;

    GPIO_init(SW2, INPUT) ; 
    GPIO_pupd(SW2, PU) ;

    // Initialize EXTI PA_4
    EXTI_init(SW2, FALL, 10);
    EXTI_init(SW1, FALL, 10);

        
    FND_init(_pinFND) ;          // Intialize FND pins and Others 
    FND_select_init(_selectFND) ; 
    pinFNDsetup(_pinFND) ;
    selectFNDsetup(_selectFND) ;

}

// global  
int count = 0 ;
int SW2pressed = 1 ;
int secondCount = 0 ;
int pause = 0 ;

// MAIN  ----------------------------------------
int main(void) {
	setup();
    int numDisplay[10]  = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} ;
    uint8_t selectFND1  = 0 ;
    uint8_t selectFND2  = 1 ;

    int elapsed = 0;

    while (1) {
        FND_select(selectFND1);
        FND_display(_pinFND, numDisplay[count]);
        delay_ms(5);

        FND_select(selectFND2);
        FND_display(_pinFND, numDisplay[secondCount]);
        delay_ms(5);

        elapsed += 10;                  // 이미 걸린 시간
        if (elapsed >= 1000) {          // 100바퀴 = 1초
            elapsed = 0;
            if (pause == 0){
                count++;
                if (count == 10) {
                    count = 0;
                    secondCount++ ;
            }
            }
        }
    }
}

// SW1 is pressed -> resets to zero
void EXTI2_IRQHandler(void) {
	if (is_pending_EXTI(SW1)) {   
		count = 0 ;
        secondCount = 0 ;
		clear_pending_EXTI(SW1);
	}
}
// SW2 is pressed -> pause the counting
void EXTI4_IRQHandler(void) {
    if (is_pending_EXTI(SW2)) {   
        pause = !pause ; 
		clear_pending_EXTI(SW2);
	}
}

