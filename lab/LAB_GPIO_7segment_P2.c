/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Haeun Yang
Created          : 05-03-2021
Modified         : 09-20-2026
Language/ver     : C++ in VS Code

Description      : Toggle multi LEDs with a button
/----------------------------------------------------------------*/

#include "ecSTM32F4v2.h"

#define BUTTON_PIN PA_4

PinName_t pinFND[8]    = {PB_0, PB_1, PB_2, PB_3, PB_4, PB_5, PB_6, PB_7} ;
PinName_t selectFND[4] = {PA_10, PA_11, PC_4, PC_3} ;

// Initialiization 
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
    RCC_HSI_init() ;                // Intialize System Clock

    GPIO_init(BUTTON_PIN, INPUT) ;  // Initialize the pushbutton pin as an input:
    GPIO_pupd(BUTTON_PIN, PU) ;

    FND_init(pinFND);               // Intialize FND pins and Others 
    FND_select_init(selectFND) ;    

    selectFNDsetup(selectFND) ;     // Intialize  Others
    pinFNDsetup(pinFND) ;   
};
	
int main(void) {
    setup();
    int numDisplay[10]  = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9} ;
    uint8_t selectFND1  = 0 ;
    uint8_t selectFND2  = 1 ;
    int buttonState = 1 ;
    int prevButtonState = 1 ; 
    int count = 0 ;
    int countTens = 0 ;

    while (1) {
        // count 버튼 누를 때마다 숫자 증가
        // BUTTON -> Active Low(0 일 때가 눌렸을 때)
        buttonState = GPIO_read(BUTTON_PIN) ;
        if (buttonState == 0 && prevButtonState == 1 && count <= 9) {
            FND_select(selectFND1);
            FND_display(pinFND, numDisplay[count]);
            count++ ;
            delay_ms_HSI(1);    
        }         		
        else if (buttonState == 0 && prevButtonState == 1 && count == 10){
            while(1){
                buttonState = GPIO_read(BUTTON_PIN) ;
                if (buttonState == 0 && prevButtonState == 1) countTens++ ;
                if (countTens == 11) {
                    countTens = 0 ;
                    count++ ;
                    break ;
                } 
                FND_select(selectFND1);
                FND_display(pinFND, numDisplay[countTens-1]);
                delay_ms_HSI(1);             		
                FND_select(selectFND2);
                FND_display(pinFND, numDisplay[1]);
                delay_ms_HSI(1);	             
                prevButtonState = buttonState;
            }
        }

        else if (buttonState == 0 && prevButtonState == 1 && count == 11){
            while(1){
                buttonState = GPIO_read(BUTTON_PIN) ;
                if (buttonState == 0 && prevButtonState == 1) countTens++ ;
                if (countTens == 11) {
                    countTens = 0 ;
                    count++ ;
                    break ;
                } 
                FND_select(selectFND1);
                FND_display(pinFND, numDisplay[countTens-1]);
                delay_ms_HSI(1);             		
                FND_select(selectFND2);
                FND_display(pinFND, numDisplay[2]);
                delay_ms_HSI(1);	             
                prevButtonState = buttonState;
            }
        }

        prevButtonState = buttonState ;
        delay_ms_HSI(100) ;
    }
}