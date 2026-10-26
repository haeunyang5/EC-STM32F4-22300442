/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Haeun Yang
Created          : 05-03-2021
Modified         : 09-21-2026
Language/ver     : C++ in VS Code

Description      : Toggle a single LED with Photodetector
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"


PinName_t _pinFND[8]    = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7} ;
PinName_t _selectFND[4] = {PA_10, PA_11, PC_4, PC_3} ;


void selectFNDsetup(PinName_t *selectFND){
    for (int i=0; i<4; i++){
        GPIO_init(selectFND[i], OUTPUT) ;  
        GPIO_otype(selectFND[i], PUSH_PULL) ;
    }
}

void pinFNDsetup(PinName_t *pinFND){
    for (int i=0; i<8; i++){
        GPIO_init(pinFND[i], OUTPUT) ;   
        GPIO_ospeed(pinFND[i], MEDIUM_SPEED) ;
        GPIO_otype(pinFND[i], PUSH_PULL) ;
    }
}

void setup(void){
    // Intialize System Clock
    RCC_HSI_init() ;    
    // Intialize FND pins and Others
    FND_init(_pinFND);
    FND_select_init(_selectFND) ;    
    // Intialize  Others
    selectFNDsetup(_selectFND) ;
    pinFNDsetup(_pinFND) ;   
};

int main(void) {
    setup();
    uint8_t numDisplay1  = 0 ;
    uint8_t numDisplay2  = 5 ;
    uint8_t selectFND1  = 0 ;
    uint8_t selectFND2  = 1 ;

    while (1) {

        FND_select(selectFND1);
        FND_display(_pinFND, numDisplay1);
        delay_ms_HSI(1);             		
        FND_select(selectFND2);
        FND_display(_pinFND, numDisplay2);
        delay_ms_HSI(1);	
    }
}


