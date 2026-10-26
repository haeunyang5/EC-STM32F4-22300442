/*----------------------------------------------------------------\
@ Embedded Controller by Young-Keun Kim - Handong Global University
Author           : Haeun Yang
Created          : 05-03-2021
Modified         : 09-14-2026
Language/ver     : C++ in VS Code

Description      : Toggle a single LED with Photodetector
/----------------------------------------------------------------*/

#include "ecRCC2.h"
#include "ecGPIO2.h"

#define LED_PIN_ PB_12
#define BUTTON_PIN_ PA_0

// Initialization
void setup(void){
    RCC_HSI_init();
    GPIO_init(BUTTON_PIN_, INPUT) ;  // Initialize the pushbutton pin as an input:
    GPIO_pupd(BUTTON_PIN_, PU) ;
    
    GPIO_init(LED_PIN_, OUTPUT) ;   // Initialze the LED pin as an output
    GPIO_ospeed(LED_PIN_, MEDIUM_SPEED) ;
    GPIO_pupd(LED_PIN_, PU) ;
    GPIO_otype(LED_PIN_, OPEN_DRAIN) ;
}

int main(void){
    setup();
    int buttonState = 0 ;

    while(1){
        // check if the pushbutton is pressed. Turn LED on/off accordingly
        // toggle (Turn on -> off -> on -> off)
        buttonState = GPIO_read(BUTTON_PIN_) ;
        if(buttonState == 1)  GPIO_write(LED_PIN_, LOW) ;
        else GPIO_write(LED_PIN_, HIGH) ;
        delay_ms_HSI(100) ;
    }
}