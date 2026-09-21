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

#define LED_PIN PB_12
#define BUTTON_PIN PA_0

// Initialization
void setup(void){
    RCC_HSI_init();
    GPIO_init(BUTTON_PIN, INPUT) ;  // Initialize the pushbutton pin as an input:
    GPIO_pupd(BUTTON_PIN, PU) ;
    
    GPIO_init(LED_PIN, OUTPUT) ;   // Initialze the LED pin as an output
    GPIO_ospeed(LED_PIN, MEDIUM_SPEED) ;
    GPIO_pupd(LED_PIN, PU) ;
    GPIO_otype(LED_PIN, OPEN_DRAIN) ;
}

int main(void){
    setup();
    int buttonState = 0 ;

    while(1){
        // check if the pushbutton is pressed. Turn LED on/off accordingly
        // toggle (Turn on -> off -> on -> off)
        buttonState = GPIO_read(BUTTON_PIN) ;
        if(buttonState == 1)  GPIO_write(LED_PIN, LOW) ;
        else GPIO_write(LED_PIN, HIGH) ;
        delay_ms_HSI(100) ;
    }
}